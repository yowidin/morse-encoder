/**
 * @file   welcome.h
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */
#ifndef INCLUDE_HW_STATES_WELCOME_H
#define INCLUDE_HW_STATES_WELCOME_H

#include <hw/states/state.h>

namespace hw {
namespace states {

class welcome : public state {
public:
   using state::state;

public:
   void activate() override;
   void deactivate() override;
   void redraw() override;

   void flip_font();

private:
   int active_screen_{0};
   volatile bool reverse_font_{true};
};

} // namespace states
} // namespace hw

#endif /* INCLUDE_HW_STATES_WELCOME_H */
