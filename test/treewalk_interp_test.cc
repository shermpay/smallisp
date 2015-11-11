#include "treewalk_interp.h"

#include <gtest/gtest.h>
#include "environment.h"
#include "specialforms.h"

namespace sl {

namespace {

#define ASSERT_NOTNULL(x) (ASSERT_NE(nullptr, x))

TEST(Environments, BindLookup) {
  interp::Treewalker interp;
  interp.Bind(Symbol::Val("x"), Int::Val(1));
  const Object *obj = interp.Lookup(Symbol::Val("x"));
  ASSERT_EQ(Int::Val(1), *obj);
  // Unbound symbol
  ASSERT_EQ((Error("")), *interp.Lookup(Symbol::Val("y")));
  interp.Bind(Symbol::Val("y"), Int::Val(2));
  ASSERT_EQ(Int::Val(2), *interp.Lookup(Symbol::Val("y")));
  interp.Bind(Symbol::Val("x"), Int::Val(3));
  ASSERT_EQ(Int::Val(3), *interp.Lookup(Symbol::Val("x")));
}

TEST(SpecialFormHandler, Define) {
  interp::Treewalker interp;
  const List expr({&specialforms::kDefine, Symbol::Get("x"), Int::Get(1)});
  const Object *obj = interp.Define(expr);
  ASSERT_NE(Error(""), *obj);
  ASSERT_EQ(Int::Val(1), *interp.Lookup(Symbol::Val("x")));
  // Redefining symbol
  ASSERT_EQ(Error(""), *interp.Define(expr));
}

TEST(SpecialFormHandler, UnsafeSet) {
  interp::Treewalker interp;
  const List expr({&specialforms::kUnsafeSet, Symbol::Get("x"), Int::Get(2)});
  // Undefined symbol
  ASSERT_EQ(Error(""), *interp.UnsafeSet(expr));
  const List def_expr({&specialforms::kDefine, Symbol::Get("x"), Int::Get(1)});
  interp.Define(def_expr);
  ASSERT_EQ(Int::Val(1), *interp.Lookup(Symbol::Val("x")));
  interp.UnsafeSet(expr);
  ASSERT_EQ(Int::Val(2), *interp.Lookup(Symbol::Val("x")));
}

TEST(Eval, EvalPrimitives) {
  interp::Treewalker interp;
  ASSERT_EQ(Int::Val(1), *interp.Eval(Int::Val(1)));
  interp.Bind(Symbol::Val("x"), Int::Val(1));
  ASSERT_EQ(Int::Val(1), *interp.Eval(Symbol::Val("x")));
  const List expr({&specialforms::kUnsafeSet, Symbol::Get("x"), Int::Get(2)});
  interp.Eval(expr);
  ASSERT_EQ(Int::Val(2), *interp.Lookup(Symbol::Val("x")));
}

}  // anonymous
}  // sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
