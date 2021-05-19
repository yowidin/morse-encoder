/**
 * @file   sending_text.h
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */
#ifndef INCLUDE_HW_STATES_SENDING_TEXT_H
#define INCLUDE_HW_STATES_SENDING_TEXT_H

#include <hw/states/state.h>

#include <string>

namespace hw {
namespace states {

class sending_text : public state {
public:
   explicit sending_text(application &app);

private:
   void do_send();
   void send_symbol(char ch, bool with_gap = true);

   void dot(bool with_gap = true);
   void dash(bool with_gap = true);

public:
   void activate() override;
   void deactivate() override;
   void redraw() override;

private:
   unsigned speed_{150};
   int tone_frequency_{750};
   std::string buffer_;
};

} // namespace states
} // namespace hw

#endif /* INCLUDE_HW_STATES_SENDING_TEXT_H */
