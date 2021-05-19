/**
 * @file   rotary_encoder.cpp
 * @author Dennis Sitelew
 * @date   Apr. 01, 2021
 */

#include <hw/rotary_encoder.h>

#include <Arduino.h>

#include <limits>

using namespace hw;

rotary_encoder::rotary_encoder(std::uint8_t clk,
                               std::uint8_t dt,
                               std::uint8_t sw)
   : clk_pin{clk}
   , dt_pin{dt}
   , sw_pin{sw} {
   // Nothing to do here
}

void rotary_encoder::setup() {
   pinMode(clk_pin, INPUT_PULLUP);
   pinMode(dt_pin, INPUT_PULLUP);
   pinMode(sw_pin, INPUT_PULLUP);

   is_pressed_ = digitalRead(sw_pin) == 0;
}

void rotary_encoder::update() {
   // Handle value changes
   auto v = read_rotary();
   if (v != 0) {
      value_ += v;
      if (rotation_handler) {
         rotation_handler(value_);
      }
   }

   // Handle clicks
   bool new_pressed = read_button();
   if (new_pressed != is_pressed_) {
      is_pressed_ = new_pressed;

      if (new_pressed) {
         Serial.print("Click\n");
         if (click_handler) {
            click_handler();
         }
      } else {
         Serial.print("Release\n");
         if (release_handler) {
            release_handler();
         }
      }
   }
}

void rotary_encoder::set_rotation_handler(rotation_handler_t rh) {
   std::swap(rotation_handler, rh);
}

void rotary_encoder::set_click_handler(click_handler_t ch) {
   std::swap(click_handler, ch);
}

void rotary_encoder::set_release_handler(release_handler_t rh) {
   std::swap(release_handler, rh);
}

/**
 * Rotating the encoder counter-clock-wise generates a negative value. This
 * method converts those negative values to the range [0, max_value), where
 * negative values are wrapped to the end of range. With this method we can
 * encode navigation in all sorts of menus where CCW rotation means jumping to
 * the end of the list and navigating backwards.
 * @param max_value List length
 * @return Normalized encoder value
 */
int rotary_encoder::get_normalized(int max_value) const {
   int circles = value_ / max_value;
   int reminder = value_ - circles * max_value;

   if (reminder < 0) {
      return max_value + reminder;
   } else {
      return reminder;
   }
}

// https://www.best-microcontroller-projects.com/rotary-encoder.html
std::int8_t rotary_encoder::read_rotary() {
   const static int8_t rot_enc_table[] = {0, 1, 1, 0, 1, 0, 0, 1,
                                          1, 0, 0, 1, 0, 1, 1, 0};

   prev_next_code_ <<= 2;
   if (digitalRead(dt_pin)) {
      prev_next_code_ |= 0x02;
   }

   if (digitalRead(clk_pin)) {
      prev_next_code_ |= 0x01;
   }

   prev_next_code_ &= 0x0f;

   // If valid then store as 16 bit data.
   if (rot_enc_table[prev_next_code_]) {
      store_ <<= 4;
      store_ |= prev_next_code_;
      if ((store_ & 0xff) == 0x2b) {
         return -1;
      }
      if ((store_ & 0xff) == 0x17) {
         return 1;
      }
   }
   return 0;
}

bool rotary_encoder::read_button() {
   bool new_pressed = (digitalRead(sw_pin) == LOW);

   auto now = millis();
   if (new_pressed != last_pressed_) {
      last_debounce_time_ = now;
   }

   decltype(now) time_diff;
   if (now < last_debounce_time_) {
      // millis counter overflow
      time_diff =
          std::numeric_limits<decltype(now)>::max() - last_debounce_time_;
      time_diff += now;
   } else {
      time_diff = now - last_debounce_time_;
   }

   last_pressed_ = new_pressed;

   // Do not register clicks with less than 50ms in between
   const decltype(now) DEBOUNCE_DELAY = 50;
   if (time_diff > DEBOUNCE_DELAY) {
      return new_pressed;
   }

   return is_pressed_;
}
