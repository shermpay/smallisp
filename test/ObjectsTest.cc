#include "Objects.h"
#include <gtest/gtest.h>

namespace {

TEST(ObjectAllocation, IntAllocation) {
  sl::Object *Integer = sl::objects::newInt(5);
  EXPECT_EQ(5, Integer->Val->Int);
}

TEST(ObjectAllocation, SymbolAllocation) {
  sl::Object *SymFoo = sl::objects::newSymbol("Foo");
  EXPECT_EQ("Foo", SymFoo->Val->Symbol->Name);
  sl::Object *SymBar = sl::objects::newSymbol("Bar");
  EXPECT_EQ("Bar", SymBar->Val->Symbol->Name);
  sl::Object *SymFooPtr = sl::objects::newSymbol("Foo");
  EXPECT_EQ("Foo", SymFooPtr->Val->Symbol->Name);
  EXPECT_EQ(SymFoo->Val->Symbol, SymFooPtr->Val->Symbol);
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
