// smallisp/environment.h - Environment Type -*- mode: C++ -*- //
#ifndef _ENVIRONMENT_DEF
#define _ENVIRONMENT_DEF

#include <unordered_map>
#include "objects.h"

namespace sl {

typedef std::unordered_map<const Symbol *, const Object *> Environment;
}

#endif
