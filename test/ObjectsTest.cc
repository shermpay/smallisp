#include "Objects.h"
#include <gtest/gtest.h>

namespace {

TEST(ObjectAllocation, IntAllocation) {
  sl::Int *integer = new sl::Int(5);
  EXPECT_EQ(5, integer->value());
}

TEST(ObjectAllocation, SymbolAllocation) {
  sl::Symbol *sym_foo = sl::Symbol::Get("Foo");
  EXPECT_EQ("Foo", sym_foo->name());
  sl::Symbol *sym_bar = sl::Symbol::Get("Bar");
  EXPECT_EQ("Bar", sym_bar->name());
  sl::Symbol *sym_foo_ptr = sl::Symbol::Get("Foo");
  EXPECT_EQ("Foo", sym_foo_ptr->name());
  EXPECT_EQ(sym_foo, sym_foo_ptr);
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
