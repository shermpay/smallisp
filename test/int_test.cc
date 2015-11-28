#include "int.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

namespace sl {
////////////////////////
// Generic Operations //
////////////////////////
TEST(Int, Allocation) {
  const Int &integer = Int::Val(5);
  EXPECT_EQ(5, integer.value());
  EXPECT_EQ(5, Int::Val(5).value());
  EXPECT_EQ(3, Int::Val(3).value());
  EXPECT_EQ(2, Int::Val(2).value());
  EXPECT_EQ(1, Int::Val(1).value());
}
TEST(Int, Equality) {
  const Int &integer = Int::Val(5);
  EXPECT_EQ(Int::Val(1), Int::Val(1));
  EXPECT_EQ(Int::Val(5), integer);
  EXPECT_NE(Int::Val(6), integer);
  EXPECT_EQ(Int::Val(7), Int::Val(7));
};

TEST(Int, TestPrinter) {
  const Int &integer = Int::Val(5);
  EXPECT_EQ("5", ::testing::PrintToString(integer));
}
TEST(Int, Hashcode) {
  EXPECT_NE(Int::Val(0).Hashcode(), Int::Val(1).Hashcode());
  EXPECT_EQ(Int::Val(0).Hashcode(), Int::Val(0).Hashcode());
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

////////////////
// Comparable //
////////////////
TEST(Int, Comparable) {
  EXPECT_GT(0, Int::Val(1).Cmp(Int::Val(2)));
  EXPECT_LT(0, Int::Val(2).Cmp(Int::Val(1)));
  EXPECT_EQ(0, Int::Val(1).Cmp(Int::Val(1)));
  EXPECT_NE(0, Int::Val(2).Cmp(Int::Val(1)));
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
