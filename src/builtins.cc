#include "builtins.h"

namespace sl {

namespace builtins {
Environment environment{
    // {Symbol::Get("add"), Int::Get(0)},     {Symbol::Get("sub"), Int::Get(0)},
    // {Symbol::Get("mul"), Int::Get(0)},     {Symbol::Get("div"), Int::Get(0)},
    // {Symbol::Get("println"), Int::Get(0)},
};

}  // namespace builtins
}  // namespace sl
