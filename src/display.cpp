/**
 * @file   display.cpp
 * @author Dennis Sitelew
 * @date   Apr. 03, 2021
 */

#include <hw/display.h>
#include <hw/util.h>

using namespace hw;

void display::setup(int sda_pin, int scl_pin) {
   Serial.print("Looking for OLED\n");
   int rc = obdI2CInit(&display_, OLED_128x64, -1, false, false, true, sda_pin,
                       scl_pin, -1, 800000L);

   if (rc != OLED_NOT_FOUND) {
      Serial.print("OLED found\n");

      // Clear the screen
      obdSetBackBuffer(&display_, back_buffer_);
      obdFill(&display_, 0, 1);
      flush();

      // Lower the brightness
      obdSetContrast(&display_, 128);
   } else {
      Serial.print("OLED not found\n");
      reset();
   }
}

void display::clear() {
   obdFill(&display_, 0, false);
}

void display::flush() {
   obdDumpBuffer(&display_, back_buffer_);
}

void display::off() {
   obdPower(&display_, false);
}

void display::on() {
   obdPower(&display_, true);
}

void display::left(const char *text,
                   font fnt,
                   int line,
                   int offset,
                   style stl) {
   obdWriteString(&display_, 0, offset, line, const_cast<char *>(text),
                  static_cast<int>(fnt), static_cast<bool>(stl), false);
}

void display::center(const char *text,
                     font fnt,
                     int line,
                     int offset,
                     style stl) {
   const auto length = strlen(text);
   const auto size = font_size(fnt);
   const auto text_width = size.x * length;

   int text_start = static_cast<int>(SCREEN_CENTER - text_width / 2);

   obdWriteString(&display_, 0, text_start + offset, line,
                  const_cast<char *>(text), static_cast<int>(fnt),
                  static_cast<bool>(stl), false);
}

void display::right(const char *text,
                    font fnt,
                    int line,
                    int offset,
                    style stl) {
   auto length = strlen(text);
   auto size = font_size(fnt);

   int text_start = static_cast<int>(SCREEN_WIDTH - size.x * length);

   obdWriteString(&display_, 0, text_start + offset, line,
                  const_cast<char *>(text), static_cast<int>(fnt),
                  static_cast<bool>(stl), false);
}

void display::text(const char *text,
                   font fnt,
                   int line,
                   int offset,
                   style stl) {
   obdWriteString(&display_, 0, offset, line, const_cast<char *>(text),
                  static_cast<int>(fnt), static_cast<bool>(stl), false);
}

display::dimensions display::font_size(font font) {
   switch (font) {
      case display::font::small:
         return {6, 8};

      case display::font::normal:
         return {8, 8};

      case display::font::medium:
         return {12, 16};

      case display::font::stretched:
         return {16, 16};

      case display::font::large:
         return {16, 32};
   }

   Serial.printf("Unexpected font: %d\n", static_cast<int>(font));
   reset();
}
