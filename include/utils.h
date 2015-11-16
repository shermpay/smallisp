//===- smallisp/utils.h - Smallisp utilities --*- mode: C++ -*-=== //

#ifndef _UTILS_DEF
#define _UTILS_DEF

#include <string>
#include "objects.h"

namespace sl {

namespace debug {

// Enable debugging if passed in true, else disable debugging.
void Enable(bool);

const Error *ErrorWithTrace(const std::string &msg);

}  // namespace debug
}  // namespace sl
#endif
