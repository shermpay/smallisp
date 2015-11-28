#include "byte.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

namespace sl {
////////////////////////
// Generic Operations //
////////////////////////
TEST(Byte, Allocation) {
  const Byte &byte = Byte::Val(5);
  EXPECT_EQ(5, byte.value());
  EXPECT_EQ(5, Byte::Val(5).value());
  EXPECT_EQ(3, Byte::Val(3).value());
  EXPECT_EQ(2, Byte::Val(2).value());
  EXPECT_EQ(1, Byte::Val(1).value());
}
TEST(Byte, Equality) {
  const Byte &byte = Byte::Val(5);
  EXPECT_EQ(Byte::Val(1), Byte::Val(1));
  EXPECT_EQ(Byte::Val(5), byte);
  EXPECT_NE(Byte::Val(6), byte);
  EXPECT_EQ(Byte::Val(7), Byte::Val(7));
};

TEST(Byte, TestPrinter) {
  const Byte &byte = Byte::Val(5);
  EXPECT_EQ("5", ::testing::PrintToString(byte));
}
TEST(Byte, Hashcode) {
  EXPECT_NE(Byte::Val(0).Hashcode(), Byte::Val(1).Hashcode());
  EXPECT_EQ(Byte::Val(0).Hashcode(), Byte::Val(0).Hashcode());
}

//////////////////
// Byte Specific //
//////////////////
TEST(Byte, Operators) {
  // +
  EXPECT_EQ(Byte::Val(1), Byte::Val(0) + Byte::Val(1));
  EXPECT_EQ(Byte::Val(0), Byte::Val(-1) + Byte::Val(1));

  // -
  EXPECT_EQ(Byte::Val(0), Byte::Val(1) - Byte::Val(1));
  EXPECT_EQ(Byte::Val(-1), Byte::Val(0) - Byte::Val(1));

  // *
  EXPECT_EQ(Byte::Val(4), Byte::Val(2) * Byte::Val(2));
  EXPECT_EQ(Byte::Val(0), Byte::Val(0) * Byte::Val(12));
  EXPECT_EQ(Byte::Val(0), Byte::Val(12) * Byte::Val(0));
  EXPECT_EQ(Byte::Val(-4), Byte::Val(-2) * Byte::Val(2));
  EXPECT_EQ(Byte::Val(4), Byte::Val(-2) * Byte::Val(-2));

  // /
  EXPECT_EQ(Byte::Val(2), Byte::Val(2) / Byte::Val(1));
  EXPECT_EQ(Byte::Val(2), Byte::Val(4) / Byte::Val(2));
  EXPECT_EQ(Byte::Val(0), Byte::Val(0) / Byte::Val(2));
  // Divide by zero is an assertion error, should be checked in library
  // Fraction division is not handled on this level
}

////////////////
// Comparable //
////////////////
TEST(Byte, Comparable) {
  EXPECT_GT(0, Byte::Val(1).Cmp(Byte::Val(2)));
  EXPECT_LT(0, Byte::Val(2).Cmp(Byte::Val(1)));
  EXPECT_EQ(0, Byte::Val(1).Cmp(Byte::Val(1)));
  EXPECT_NE(0, Byte::Val(2).Cmp(Byte::Val(1)));
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
