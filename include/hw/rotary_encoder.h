/**
 * @file   rotary_encoder.h
 * @author Dennis Sitelew
 * @date   Apr. 01, 2021
 */
#ifndef INCLUDE_HW_ROTARY_ENCODER_H
#define INCLUDE_HW_ROTARY_ENCODER_H

#include <cstdint>
#include <functional>

namespace hw {

// https://cb-electronics.com/products/ky-040/ (20 detents)
class rotary_encoder {
public:
   using rotation_handler_t = std::function<void(int)>;
   using click_handler_t = std::function<void()>;
   using release_handler_t = std::function<void()>;

public:
   rotary_encoder(std::uint8_t clk, std::uint8_t dt, std::uint8_t sw);

public:
   void setup();
   void update();

   void set_rotation_handler(rotation_handler_t rh);
   void set_click_handler(click_handler_t ch);
   void set_release_handler(release_handler_t rh);

   void reset_value() { value_ = 0; }
   int get_value() const { return value_; }

   int get_normalized(int max_value) const;

private:
   std::int8_t read_rotary();
   bool read_button();

private:
   std::uint8_t clk_pin;
   std::uint8_t dt_pin;
   std::uint8_t sw_pin;

   int value_{0};
   bool is_pressed_{false};

   rotation_handler_t rotation_handler{};
   click_handler_t click_handler{};
   release_handler_t release_handler{};

   //! Rotary debounce
   uint8_t prev_next_code_{0};
   uint16_t store_{0};

   //! Button debounce
   unsigned long last_debounce_time_{0};
   bool last_pressed_{false};
};

} // namespace hw

#endif /* INCLUDE_HW_ROTARY_ENCODER_H */
