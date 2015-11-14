#include "treewalk_interp.h"

#include <gtest/gtest.h>
#include "environment.h"
#include "specialforms.h"

namespace sl {

namespace {

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

TEST(SpecialForm, Def) {
  interp::Treewalker interp;
  const List expr({&specialforms::kDef, Symbol::Get("x"), Int::Get(1)});
  const Object *obj = interp.Define(expr);
  ASSERT_NE(Error(""), *obj);
  ASSERT_EQ(Int::Val(1), *interp.Lookup(Symbol::Val("x")));
  // Redefining symbol
  ASSERT_EQ(Error(""), *interp.Define(expr));
}

TEST(SpecialForm, UnsafeSet) {
  interp::Treewalker interp;
  const List expr({&specialforms::kUnsafeSet, Symbol::Get("x"), Int::Get(2)});
  // Undefined symbol
  ASSERT_EQ(Error(""), *interp.UnsafeSet(expr));
  const List def_expr({&specialforms::kDef, Symbol::Get("x"), Int::Get(1)});
  interp.Define(def_expr);
  ASSERT_EQ(Int::Val(1), *interp.Lookup(Symbol::Val("x")));
  interp.UnsafeSet(expr);
  ASSERT_EQ(Int::Val(2), *interp.Lookup(Symbol::Val("x")));
}

TEST(SpecialForm, Lambda) {
  interp::Treewalker interp;
  const List id_lambda_expr{
      {&specialforms::kLambda, new List{Symbol::Get("x")}, Symbol::Get("x")}};
  const Object *id_fn_obj = interp.Eval(id_lambda_expr);
  ASSERT_EQ(Type::kFunction, id_fn_obj->GetType());
  const Function &id_fn = *static_cast<const Function *>(id_fn_obj);
  ASSERT_EQ(List{Symbol::Get("x")}, id_fn.params());
  ASSERT_EQ(List{Symbol::Get("x")}, id_fn.body());
}

TEST(Functions, Call) {
  interp::Treewalker interp;
  const List id_lambda_expr{
      {&specialforms::kLambda, new List{Symbol::Get("x")}, Symbol::Get("x")}};
  const Object *id_fn_obj = interp.Eval(id_lambda_expr);
  ASSERT_EQ(Type::kFunction, id_fn_obj->GetType());
  const Function &id_fn = *static_cast<const Function *>(id_fn_obj);
  const Object *result = interp.Call(id_fn, List{Int::Get(1)});
  ASSERT_EQ(Int::Val(1), *result);
}

TEST(Eval, EvalPrimitives) {
  interp::Treewalker interp;
  ASSERT_EQ(Int::Val(1), *interp.Eval(Int::Val(1)));
  interp.Eval(List{&specialforms::kDef, Symbol::Get("x"), Int::Get(1)});
  ASSERT_EQ(Int::Val(1), *interp.Eval(Symbol::Val("x")));
  const List expr({&specialforms::kUnsafeSet, Symbol::Get("x"), Int::Get(2)});
  interp.Eval(expr);
  ASSERT_EQ(Int::Val(2), *interp.Lookup(Symbol::Val("x")));
  const Object *err =
      interp.Eval(List{new List({&specialforms::kLambda, kNil()})});
  ASSERT_EQ(Error(""), *err);
  err = interp.Eval(List{new List({&specialforms::kLambda})});
  ASSERT_EQ(Error(""), *err);
}

}  // anonymous
}  // sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
