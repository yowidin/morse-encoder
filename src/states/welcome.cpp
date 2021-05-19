/**
 * @file   welcome.cpp
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */

#include <hw/application.h>
#include <hw/states/welcome.h>

extern "C" {
#include <user_interface.h>
}

using namespace hw::states;

namespace {

os_timer_t g_timer;

static void timer_func(void *ptr) {
   auto obj = reinterpret_cast<welcome *>(ptr);
   obj->flip_font();
}

} // namespace

void welcome::activate() {
   os_timer_setfn(&g_timer, (os_timer_func_t *)timer_func, this);
   os_timer_arm(&g_timer, 500, true);

   app_->get_encoder().set_release_handler([this] {
      ++active_screen_;

      app_->get_display().clear();
      app_->mark_dirty();

      if (active_screen_ == 2) {
         app_->set_state(application::state::text_input);
      }
   });
}

void welcome::deactivate() {
   os_timer_disarm(&g_timer);
}

void welcome::flip_font() {
   reverse_font_ = !reverse_font_;
   app_->mark_dirty();
}

void welcome::redraw() {
   using font = display::font;
   using style = display::style;

   auto &screen = app_->get_display();
   auto prompt = [&] {
      auto stl = reverse_font_ ? style::inverted : style::normal;
      screen.center("click knob", font::small, 6, 2, stl);
      screen.center("to continue", font::small, 7, 2, stl);
   };

   if (active_screen_ == 0) {
      screen.left("flcl", font::small, 1);
      screen.right("0.1", font::small, 1);
      screen.center("Morse", font::large, 0, 4);
      screen.center("Encoder", font::large, 3, 4);

      prompt();
   } else if (active_screen_ == 1) {
      screen.text(" Rotate the knob", font::normal, 0);
      screen.text("to select", font::normal, 1);
      screen.text("letters. Click", font::normal, 2);
      screen.text("the knob to con-", font::normal, 3);
      screen.text("firm selection.", font::normal, 4);

      prompt();
   }

   screen.flush();
}
