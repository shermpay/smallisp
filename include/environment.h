// smallisp/environment.h - Environment Type -*- mode: C++ -*- //
#ifndef _ENVIRONMENT_DEF
#define _ENVIRONMENT_DEF

#include <unordered_map>

namespace sl {

typedef std::unordered_map<const class Symbol *, const class Object *> Environment;
}

#endif
