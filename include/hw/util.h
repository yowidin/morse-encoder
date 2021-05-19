/**
 * @file   util.h
 * @author Dennis Sitelew
 * @date   Apr. 03, 2021
 */
#ifndef INCLUDE_HW_UTIL_H
#define INCLUDE_HW_UTIL_H

#include "user_interface.h"

namespace hw {
[[noreturn]] inline void reset() {
   Serial.flush();
   delay(1000);
   ESP.reset();
   abort();
}
} // namespace hw

#endif /* INCLUDE_HW_UTIL_H */
