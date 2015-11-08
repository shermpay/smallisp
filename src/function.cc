#include "function.h"

#include "objects.h"
#include "treewalk_interp.h"

namespace sl {

const Frame *Function::MakeFrame(const List *args) const {
  const ConsC *cur = params()->head();
  Environment locals;
  for (const Object &o : *args) {
    const Symbol *param_sym = static_cast<const Symbol *>(cur->car());
    locals.insert({param_sym, &o});
    cur = static_cast<const ConsC *>(cur->cdr());
  }
  return new Frame({locals, body()});
}

}  // namespace sl
