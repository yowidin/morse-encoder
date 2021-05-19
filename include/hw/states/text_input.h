/**
 * @file   text_input.h
 * @author Dennis Sitelew
 * @date   Apr. 02, 2021
 */
#ifndef INCLUDE_HW_STATES_TEXT_INPUT_H
#define INCLUDE_HW_STATES_TEXT_INPUT_H

#include <hw/states/state.h>
#include <string>

namespace hw {
namespace states {

class text_input : public state {
public:
   explicit text_input(application &app);

public:
   void activate() override;
   void deactivate() override;
   void redraw() override;

   void flip_font();

   const std::string &get_text() const { return text_; }
   void clear_text() { text_.clear(); }

private:
   std::string text_{""};
   std::string current_symbol_{};
   volatile bool reverse_font_{true};
};

} // namespace states
} // namespace hw

#endif /* INCLUDE_HW_STATES_TEXT_INPUT_H */
