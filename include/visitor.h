// smallisp/visitor.h - Defines the visitor interface -*- mode: C++ -*- //
#ifndef _VISITOR_DEF
#define _VISITOR_DEF

namespace sl {

// TODO: Make the return a template type
class Visitor {
 public:
  virtual ~Visitor(){};
  virtual const class Object &Visit(const class Byte &) = 0;
  virtual const class Object &Visit(const class Int &) = 0;
  virtual const class Object &Visit(const class Bool &) = 0;
  virtual const class Object &Visit(const class Symbol &) = 0;
  virtual const class Object &Visit(const struct Void &) = 0;
  virtual const class Object &Visit(const class Error &) = 0;
  virtual const class Object &Visit(const class ConsC &) = 0;
  virtual const class Object &Visit(const class List &) = 0;
  virtual const class Object &Visit(const class Array &) = 0;
  virtual const class Object &Visit(const struct Nil &) = 0;
  virtual const class Object &Visit(const class Callable &) = 0;
};

}  // namespace sl
#endif
