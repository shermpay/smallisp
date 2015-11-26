#include "symbol.h"
#include <gtest/gtest.h>

namespace sl {

////////////////////////
// Generic Operations //
////////////////////////
TEST(ObjectAllocation, SymbolAllocation) {
  const Symbol &sym_foo = Symbol::Val("Foo");
  EXPECT_EQ("Foo", sym_foo.name());
  const Symbol &sym_bar = Symbol::Val("Bar");
  EXPECT_EQ("Bar", sym_bar.name());
  const Symbol &sym_foo_ptr = Symbol::Val("Foo");
  EXPECT_EQ("Foo", sym_foo_ptr.name());
  EXPECT_EQ(sym_foo, sym_foo_ptr);
}

TEST(ObjectEquality, SymbolEquality) {
  const Symbol &sym_foo = Symbol::Val("foo");
  EXPECT_EQ(Symbol::Val("foo"), sym_foo);
  EXPECT_NE(Symbol::Val("bar"), sym_foo);
}

TEST(ObjectTestPrinter, SymbolTestPrinter) {
  const Symbol &foo = Symbol::Val("foo");
  EXPECT_EQ("foo", ::testing::PrintToString(foo));
}

TEST(Hashing, SymbolHashcode) {
  EXPECT_NE(Symbol::Val("a").Hashcode(), Symbol::Val("A").Hashcode());
  EXPECT_EQ(Symbol::Val("a").Hashcode(), Symbol::Val("a").Hashcode());
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
