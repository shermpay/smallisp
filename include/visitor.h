// smallisp/visitor.h - Defines the visitor interface -*- mode: C++ -*- //
#ifndef _VISITOR_DEF
#define _VISITOR_DEF

#include "objects.h"

namespace sl {

// TODO: Make the return a template type
class Visitor {
 public:
  virtual ~Visitor(){};
  virtual const Object &Visit(const Byte &) = 0;
  virtual const Object &Visit(const Int &) = 0;
  virtual const Object &Visit(const Bool &) = 0;
  virtual const Object &Visit(const Symbol &) = 0;
  virtual const Object &Visit(const Void &) = 0;
  virtual const Object &Visit(const Error &) = 0;
  virtual const Object &Visit(const ConsC &) = 0;
  virtual const Object &Visit(const List &) = 0;
  virtual const Object &Visit(const Nil &) = 0;
  virtual const Object &Visit(const Callable &) = 0;
};

}  // namespace sl
#endif
