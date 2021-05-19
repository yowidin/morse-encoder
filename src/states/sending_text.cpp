/**
 * @file   sending_text.cpp
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */

#include <hw/application.h>
#include <hw/states/sending_text.h>
#include <hw/util.h>

#include <user_interface.h>

#include <unordered_map>

namespace {
std::unordered_map<char, std::string> *SYMBOLS;
}

using namespace hw::states;

sending_text::sending_text(application &app)
   : state{app} {
   buffer_.reserve(3);

   pinMode(D0, OUTPUT);
   pinMode(D8, OUTPUT);

   digitalWrite(D0, 1);

   if (!SYMBOLS) {
      SYMBOLS = new std::unordered_map<char, std::string>{
          {'a', ".-"},     {'b', "-..."},   {'c', "-.-."},    {'d', "-.."},
          {'e', "."},      {'f', "..-."},   {'g', "..-."},    {'h', "...."},
          {'i', ".."},     {'j', ".---"},   {'k', "-.-"},     {'l', ".-.."},
          {'m', "--"},     {'n', "-."},     {'o', "---"},     {'p', ".--."},
          {'q', "--.-"},   {'r', ".-."},    {'s', "..."},     {'t', "-"},
          {'u', "..-"},    {'v', "...-"},   {'w', ".--"},     {'x', "-..-"},
          {'y', "-.--"},   {'z', "--.."},   {'0', "-----"},   {'1', ".----"},
          {'2', "..---"},  {'3', "...--"},  {'4', "....-"},   {'5', "....."},
          {'6', "-...."},  {'7', "--..."},  {'8', "---.."},   {'9', "----."},
          {'.', ".-.-.-"}, {',', "--..--"}, {'?', "..--.."},  {'\'', ".----."},
          {'!', "-.-.--"}, {'/', "-..-."},  {'(', "-.--."},   {')', "-.--.-"},
          {':', "---..."}, {';', "-.-.-."}, {'=', "-...-"},   {'+', ".-.-."},
          {'-', "-....-"}, {'_', "..--.-"}, {'\"', ".-..-."}, {'$', "...-..-"},
          {'@', ".--.-."}, {' ', "w"}};
   }
}

void sending_text::activate() {
   auto &encoder = app_->get_encoder();
   encoder.set_rotation_handler([](int v) {});
   encoder.set_click_handler([]() {});
   encoder.set_release_handler([]() {});

   // Hijack the update function and perform all sending inside the do_send
   do_send();
}

void sending_text::deactivate() {
   // Nothing to do here
}

void sending_text::redraw() {
   // Nothing to do here
}

void sending_text::dot(bool with_gap) {
   tone(D8, tone_frequency_);
   digitalWrite(D0, 0);
   delay(speed_);
   digitalWrite(D0, 1);
   noTone(D8);

   if (with_gap) {
      delay(speed_);
   }
}

void sending_text::dash(bool with_gap) {
   tone(D8, tone_frequency_);
   digitalWrite(D0, 0);
   delay(speed_ * 3);
   digitalWrite(D0, 1);
   noTone(D8);

   if (with_gap) {
      delay(speed_);
   }
}

void sending_text::send_symbol(char ch, bool with_gap) {
   auto &screen = app_->get_display();

   buffer_.resize(1);
   buffer_[0] = ch;

   auto it = SYMBOLS->find(ch);
   if (it == SYMBOLS->end()) {
      Serial.print("Unexpected symbol: ");
      Serial.print(ch);
      reset();
   }

   const auto &encoding = it->second;

   screen.clear();
   screen.left(buffer_.c_str(), display::font::medium, 0, 8);
   screen.left(encoding.c_str(), display::font::medium, 2, 8);
   screen.flush();

   for (unsigned i = 0; i < encoding.length(); ++i) {
      auto offset = static_cast<int>(8 + 12 * i);
      obdRectangle(*screen, 0, 32, offset, 48, 0, 1);
      screen.left("^", display::font::medium, 4, offset);
      screen.flush();

      auto s = encoding[i];
      auto inner_gap = true; // (i != encoding.length() - 1); // No gap for last symbol
      if (s == 'w') {
         // Space
         delay(speed_ * 7);
      } else {
         if (s == '.') {
            dot(inner_gap);
         } else {
            dash(inner_gap);
         }
      }
   }

   if (with_gap) {
      delay(speed_ * 3);
   }
}

void sending_text::do_send() {
   auto &text = app_->get_text();
   for (unsigned i = 0; i < text.length(); ++i) {
      auto ch = text[i];
      auto with_gap = true; // (i != text.length() - 1); // No gap for last element
      send_symbol(ch, with_gap);
   }

   auto &screen = app_->get_display();
   screen.clear();
   screen.center("DONE", display::font::large, 3);
   screen.flush();

   // End of Work:
   dot();
   dot();
   dot();
   dash();
   dot();
   dash();

   app_->clear_text();
   app_->set_state(application::state::text_input);
}
