/**
 * @file   display.h
 * @author Dennis Sitelew
 * @date   Apr. 03, 2021
 */
#ifndef INCLUDE_HW_DISPLAY_H
#define INCLUDE_HW_DISPLAY_H

#include <Arduino.h>
#include <OneBitDisplay.h>

namespace hw {

class display {
public:
   struct dimensions {
      std::uint8_t x, y;
   };

   enum class font {
      small = FONT_6x8,
      normal = FONT_8x8,
      medium = FONT_12x16,
      stretched = FONT_16x16,
      large = FONT_16x32,
   };

   enum class style {
      normal = false,
      inverted = true,
   };

   static constexpr int SCREEN_WIDTH = 128;
   static constexpr int SCREEN_CENTER = SCREEN_WIDTH / 2;
   static constexpr int SCREEN_HEIGHT = 64;

public:
   void setup(int sda_pin, int scl_pin);

   OBDISP *operator*() { return &display_; }

   void clear();
   void flush();

   void off();
   void on();

public:
   static dimensions font_size(font font);

public:
   void left(const char *text,
             font fnt,
             int line,
             int offset = 0,
             style stl = style::normal);
   void center(const char *text,
               font fnt,
               int line,
               int offset = 0,
               style stl = style::normal);
   void right(const char *text,
              font fnt,
              int line,
              int offset = 0,
              style stl = style::normal);
   void text(const char *text,
             font fnt,
             int line,
             int offset = 0,
             style stl = style::normal);

private:
   std::uint8_t back_buffer_[1024]{};
   OBDISP display_{};
};

} // namespace hw

#endif /* INCLUDE_HW_DISPLAY_H */
