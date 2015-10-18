#include "objects.h"
#include <gtest/gtest.h>
#include <iostream>

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
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
