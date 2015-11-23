#include "objects.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

namespace sl {

TEST(Type, TypeNameMap) {
  std::stringstream ss;
  ss << Type::kInt;
  std::string str;
  ss >> str;
  EXPECT_EQ("Int", str);
}

////////////////////////
// Generic Operations //
////////////////////////
TEST(ObjectAllocation, IntAllocation) {
  const Int &integer = Int::Val(5);
  EXPECT_EQ(5, integer.value());
  EXPECT_EQ(5, Int::Val(5).value());
  EXPECT_EQ(3, Int::Val(3).value());
  EXPECT_EQ(2, Int::Val(2).value());
  EXPECT_EQ(1, Int::Val(1).value());
}

TEST(ObjectAllocation, Bool) {
  const Bool &t = True();
  ASSERT_TRUE(t.value());
  const Bool &f = False();
  ASSERT_FALSE(f.value());
  // Checks if it is a singleton
  ASSERT_EQ(&True(), &True());
  ASSERT_EQ(&False(), &False());
}

TEST(ObjectAllocation, BoolMacro) {
  const Bool &t = TRUE;
  ASSERT_TRUE(t.value());
  const Bool &f = FALSE;
  ASSERT_FALSE(f.value());
}

TEST(ObjectAllocation, SymbolAllocation) {
  const Symbol &sym_foo = Symbol::Val("Foo");
  EXPECT_EQ("Foo", sym_foo.name());
  const Symbol &sym_bar = Symbol::Val("Bar");
  EXPECT_EQ("Bar", sym_bar.name());
  const Symbol &sym_foo_ptr = Symbol::Val("Foo");
  EXPECT_EQ("Foo", sym_foo_ptr.name());
  EXPECT_EQ(sym_foo, sym_foo_ptr);
}

TEST(ObjectEquality, IntEquality) {
  const Int &integer = Int::Val(5);
  EXPECT_EQ(Int::Val(1), Int::Val(1));
  EXPECT_EQ(Int::Val(5), integer);
  EXPECT_NE(Int::Val(6), integer);
  EXPECT_EQ(Int::Val(7), Int::Val(7));
};

TEST(ObjectEquality, BoolEquality) {
  EXPECT_EQ(True(), True());
  EXPECT_EQ(False(), False());
  EXPECT_NE(True(), False());
  EXPECT_NE(False(), True());
  EXPECT_EQ(TRUE, TRUE);
};

TEST(ObjectEquality, SymbolEquality) {
  const Symbol &sym_foo = Symbol::Val("foo");
  EXPECT_EQ(Symbol::Val("foo"), sym_foo);
  EXPECT_NE(Symbol::Val("bar"), sym_foo);
}

TEST(ObjectTestPrinter, IntTestPrinter) {
  const Int &integer = Int::Val(5);
  EXPECT_EQ("5", ::testing::PrintToString(integer));
}

TEST(ObjectTestPrinter, BoolTestPrinter) {
  EXPECT_EQ("true", ::testing::PrintToString(TRUE));
  EXPECT_EQ("false", ::testing::PrintToString(FALSE));
}

TEST(ObjectTestPrinter, SymbolTestPrinter) {
  const Symbol &foo = Symbol::Val("foo");
  EXPECT_EQ("foo", ::testing::PrintToString(foo));
}

TEST(Hashing, IntHashcode) {
  EXPECT_NE(Int::Val(0).Hashcode(), Int::Val(1).Hashcode());
  EXPECT_EQ(Int::Val(0).Hashcode(), Int::Val(0).Hashcode());
}

TEST(Hashing, SymbolHashcode) {
  EXPECT_NE(Symbol::Val("a").Hashcode(), Symbol::Val("A").Hashcode());
  EXPECT_EQ(Symbol::Val("a").Hashcode(), Symbol::Val("a").Hashcode());
}

TEST(Hashing, ObjectHashFn) {
  HashFn<Object> hash_fn;
  EXPECT_EQ(hash_fn(Symbol::Val("a")), hash_fn(Symbol::Val("a")));
}

TEST(Hashing, ObjectInHashSet) {
  std::unordered_set<std::reference_wrapper<const Object>, HashFn<Object>>
      obj_set;
  obj_set.insert(Int::Val(1));
  obj_set.insert(Symbol::Val("foo"));
  EXPECT_EQ(2, obj_set.size());
  std::unordered_set<std::reference_wrapper<const Symbol>, HashFn<Object>>
      sym_set;
  sym_set.insert(Symbol::Val("foo"));
  sym_set.insert(Symbol::Val("bar"));
  EXPECT_EQ(2, sym_set.size());
  std::unordered_map<Symbol, int, HashFn<Object>> sym_map{
      {Symbol::Val("bar"), 1}};
  sym_map.insert({Symbol::Val("foo"), 1});
  sym_map.insert({Symbol::Val("bar"), 1});
  EXPECT_EQ(3, sym_map.size());
}

//////////////////
// Int Specific //
//////////////////
TEST(Int, Operators) {
  // +
  EXPECT_EQ(Int::Val(1), Int::Val(0) + Int::Val(1));
  EXPECT_EQ(Int::Val(0), Int::Val(-1) + Int::Val(1));

  // -
  EXPECT_EQ(Int::Val(0), Int::Val(1) - Int::Val(1));
  EXPECT_EQ(Int::Val(-1), Int::Val(0) - Int::Val(1));

  // *
  EXPECT_EQ(Int::Val(4), Int::Val(2) * Int::Val(2));
  EXPECT_EQ(Int::Val(0), Int::Val(0) * Int::Val(12));
  EXPECT_EQ(Int::Val(0), Int::Val(12) * Int::Val(0));
  EXPECT_EQ(Int::Val(-4), Int::Val(-2) * Int::Val(2));
  EXPECT_EQ(Int::Val(4), Int::Val(-2) * Int::Val(-2));

  // /
  EXPECT_EQ(Int::Val(2), Int::Val(2) / Int::Val(1));
  EXPECT_EQ(Int::Val(2), Int::Val(4) / Int::Val(2));
  EXPECT_EQ(Int::Val(0), Int::Val(0) / Int::Val(2));
  // Divide by zero is an assertion error, should be checked in library
  // Fraction division is not handled on this level
}

///////////////////
// Bool Specific //
///////////////////
TEST(Bool, ExplicitConversion) {
  EXPECT_TRUE(bool(TRUE));
  EXPECT_FALSE(bool(FALSE));
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
