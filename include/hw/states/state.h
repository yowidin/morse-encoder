/**
 * @file   state.h
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */
#ifndef INCLUDE_HW_STATES_STATE_H
#define INCLUDE_HW_STATES_STATE_H

namespace hw {

class application;

namespace states {

class state {
public:
   state(application &app)
      : app_{&app} {
      // Nothing to do here
   }

   virtual ~state() = default;

public:
   virtual void activate() = 0;
   virtual void deactivate() = 0;
   virtual void redraw() = 0;

protected:
   application *app_;
};

} // namespace states
} // namespace hw

#endif /* INCLUDE_HW_STATES_STATE_H */
