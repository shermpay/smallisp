#include "objects.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

namespace sl {

TEST(ObjectAllocation, IntAllocation) {
  Int *integer = Int::Get(5);
  EXPECT_EQ(5, integer->value());
}

TEST(ObjectAllocation, SymbolAllocation) {
  Symbol *sym_foo = Symbol::Get("Foo");
  EXPECT_EQ("Foo", sym_foo->name());
  Symbol *sym_bar = Symbol::Get("Bar");
  EXPECT_EQ("Bar", sym_bar->name());
  Symbol *sym_foo_ptr = Symbol::Get("Foo");
  EXPECT_EQ("Foo", sym_foo_ptr->name());
  EXPECT_EQ(sym_foo, sym_foo_ptr);
}

TEST(ObjectEquality, IntEquality) {
  Int *integer = Int::Get(5);
  EXPECT_EQ(Int::Get(5), integer);
  EXPECT_NE(Int::Get(6), integer);
};

TEST(ObjectEquality, SymbolEquality) {
  Symbol *sym_foo = Symbol::Get("foo");
  EXPECT_EQ(Symbol::Get("foo"), sym_foo);
  EXPECT_NE(Symbol::Get("bar"), sym_foo);
  EXPECT_EQ(*Symbol::Get("foo"), *sym_foo);
  EXPECT_NE(*Symbol::Get("bar"), *sym_foo);
}

TEST(ObjectTestPrinter, IntTestPrinter) {
  Int *integer = Int::Get(5);
  EXPECT_EQ("5", ::testing::PrintToString(*integer));
}

TEST(ObjectTestPrinter, SymbolTestPrinter) {
  const Symbol *foo = Symbol::Get("foo");
  EXPECT_EQ("foo", ::testing::PrintToString(*foo));
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
