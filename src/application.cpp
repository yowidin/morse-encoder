/**
 * @file   application.cpp
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */

#include <hw/application.h>
#include <hw/util.h>

using namespace hw;

extern application *g_app;

// https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
application::application()
   : encoder_{D7, D6, D5}
   , display_{}
   , welcome_state_{*this}
   , text_input_state_{*this}
   , sending_text_state_{*this}
   , text_sent_state_{*this} {
   Serial.begin(115200);

   encoder_.setup();
   display_.setup(D2, D1);

   active_state_->activate();
}

void application::update() {
   if (need_redraw_) {
      active_state_->redraw();
      need_redraw_ = false;
   }

   encoder_.update();
}

void application::set_state(state state) {
   need_redraw_ = true;
   active_state_->deactivate();
   active_state_ = &state_from_enum(state);
   active_state_->activate();
}

states::state &application::state_from_enum(state value) {
   switch (value) {
      case state::welcome:
         return welcome_state_;

      case state::text_input:
         return text_input_state_;

      case state::sending_text:
         return sending_text_state_;

      case state::text_sent:
         return text_sent_state_;
   }

   Serial.print("Invalid state value: ");
   Serial.print(static_cast<int>(value));

   reset();
}