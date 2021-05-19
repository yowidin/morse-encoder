/**
 * @file   application.h
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */
#ifndef INCLUDE_HW_APPLICATION_H
#define INCLUDE_HW_APPLICATION_H

#include <Arduino.h>

#include <hw/rotary_encoder.h>
#include <hw/display.h>

#include <hw/states/welcome.h>
#include <hw/states/text_input.h>
#include <hw/states/sending_text.h>
#include <hw/states/text_sent.h>

namespace hw {

class application {
public:
   enum class state {
      welcome,
      text_input,
      sending_text,
      text_sent,
   };

public:
   application();

public:
   void update();

   void set_state(state state);

   states::state &state_from_enum(state value);

   display &get_display() { return display_; }
   hw::rotary_encoder &get_encoder() { return encoder_; }

   void mark_dirty() { need_redraw_ = true; }

   const std::string &get_text() const { return text_input_state_.get_text(); }
   void clear_text() { text_input_state_.clear_text(); }

private:
   bool need_redraw_{true};

   hw::rotary_encoder encoder_;
   hw::display display_;

   states::welcome welcome_state_;
   states::text_input text_input_state_;
   states::sending_text sending_text_state_;
   states::text_sent text_sent_state_;

   states::state *active_state_{&welcome_state_};
};

}

#endif /* INCLUDE_HW_APPLICATION_H */
