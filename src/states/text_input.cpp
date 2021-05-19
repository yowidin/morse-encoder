/**
 * @file   text_input.cpp
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */

#include <hw/application.h>
#include <hw/states/text_input.h>

#include <user_interface.h>

using namespace hw::states;

namespace {
const char *SYMBOLS[] = {
    "a", "b", "c", "d", "e", "f", "g", "h", "i",  "j", "k", "l",  "m",   "n",
    "o", "p", "q", "r", "s", "t", "u", "v", "w",  "x", "y", "z",  "0",   "1",
    "2", "3", "4", "5", "6", "7", "8", "9", ".",  ",", "?", "\'", "!",   "/",
    "(", ")", ":", ";", "=", "+", "-", "_", "\"", "$", "@", " ",  "ret", "del",
};
const int NUM_SYMBOLS = 56;

constexpr int DISPLAY_WIDTH = 128;
constexpr int DISPLAY_HEIGHT = 64;
constexpr int SYMBOL_WIDTH = 8;
constexpr int SYMBOL_HEIGHT = 8;
constexpr int SYMBOLS_PER_LINE = DISPLAY_WIDTH / SYMBOL_WIDTH;
constexpr int HEADER_LINES = 2;
constexpr int MAX_TEXT_LENGTH =
    ((DISPLAY_WIDTH * DISPLAY_HEIGHT) / (SYMBOL_WIDTH * SYMBOL_HEIGHT)) -
    (HEADER_LINES * SYMBOLS_PER_LINE);

os_timer_t g_timer;

void timer_func(void *ptr) {
   auto obj = reinterpret_cast<text_input *>(ptr);
   obj->flip_font();
}

} // namespace

text_input::text_input(application &app)
   : state{app} {
   text_.reserve(64);
   current_symbol_.reserve(4);
}

void text_input::flip_font() {
   reverse_font_ = !reverse_font_;
   app_->mark_dirty();
}

void text_input::activate() {
   os_timer_setfn(&g_timer, (os_timer_func_t *)timer_func, this);
   os_timer_arm(&g_timer, 500, true);

   obdSetTextWrap(*app_->get_display(), true);

   auto &encoder = app_->get_encoder();
   encoder.set_rotation_handler([&](int v) { app_->mark_dirty(); });
   encoder.set_release_handler([&] {
      auto handle_return = [&] {
         app_->set_state(application::state::sending_text);
      };

      if (current_symbol_.length() > 1) {
         // Complex symbol (delete or enter)
         if (current_symbol_ == "del" && !text_.empty()) {
            text_.resize(text_.length() - 1);
         }

         if (current_symbol_ == "ret") {
            handle_return();
            return;
         }
      } else if (text_.length() == (MAX_TEXT_LENGTH - 1)) {
         // Too much text - act as if enter is pressed
         handle_return();
         return;
      } else {
         text_ += current_symbol_;
      }

      app_->mark_dirty();
   });
}

void text_input::deactivate() {
   obdSetTextWrap(*app_->get_display(), false);
   os_timer_disarm(&g_timer);
}

void text_input::redraw() {
   using font = display::font;
   using style = display::style;
   auto stl = reverse_font_ ? style::inverted : style::normal;

   auto &screen = app_->get_display();

   screen.clear();
   screen.left("Enter your text:", font::normal, 0);

   current_symbol_ = SYMBOLS[app_->get_encoder().get_normalized(NUM_SYMBOLS)];

   font symbol_font = font::normal;
   if (current_symbol_.length() > 1) {
      // Complex symbol
      symbol_font = font::small;
   }

   auto offset = static_cast<int>(8 * (text_.length() % SYMBOLS_PER_LINE));
   auto line_offset = static_cast<int>(text_.length() / SYMBOLS_PER_LINE);

   screen.left(text_.c_str(), font::normal, HEADER_LINES);

   if (current_symbol_.length() > 1) {
      // Complex symbol - encoding is pretty funny: it's column based
      const std::array<std::uint8_t, 8> del = {0x04, 0x0E, 0x15, 0x04,
                                               0x04, 0x04, 0x04, 0x04};

      const std::array<std::uint8_t, 8> ret = {0x20, 0x70, 0xA8, 0x20,
                                               0x20, 0x20, 0x3E, 0x00};
      std::array<std::uint8_t, 8> symbol = current_symbol_ == "del" ? del : ret;
      if (reverse_font_) {
         std::transform(std::begin(symbol), std::end(symbol),
                        std::begin(symbol), [](std::uint8_t v) { return ~v; });
      }
      obdDrawGFX(*screen, const_cast<std::uint8_t *>(symbol.data()), 0, 0,
                 offset, HEADER_LINES + line_offset, 8, 1, 1);
   } else {
      screen.left(current_symbol_.c_str(), symbol_font,
                  HEADER_LINES + line_offset, offset, stl);
   }

   screen.flush();
}
