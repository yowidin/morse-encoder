/**
 * @file   text_sent.h
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */
#ifndef INCLUDE_HW_STATES_TEXT_SENT_H
#define INCLUDE_HW_STATES_TEXT_SENT_H

#include <hw/states/state.h>

namespace hw {
namespace states {

class text_sent : public state {
public:
   explicit text_sent(application &app);

public:
   void activate() override;
   void deactivate() override;
   void redraw() override;
};

} // namespace states
} // namespace hw

#endif /* INCLUDE_HW_STATES_TEXT_SENT_H */
