#include "bool.h"
#include <gtest/gtest.h>

namespace sl {

TEST(Bool, Allocation) {
  const Bool &t = True();
  ASSERT_TRUE(t.value());
  const Bool &f = False();
  ASSERT_FALSE(f.value());
  // Checks if it is a singleton
  ASSERT_EQ(&True(), &True());
  ASSERT_EQ(&False(), &False());
}

TEST(Bool, Macros) {
  const Bool &t = TRUE;
  ASSERT_TRUE(t.value());
  const Bool &f = FALSE;
  ASSERT_FALSE(f.value());
}
TEST(Bool, Equality) {
  EXPECT_EQ(True(), True());
  EXPECT_EQ(False(), False());
  EXPECT_NE(True(), False());
  EXPECT_NE(False(), True());
  EXPECT_EQ(TRUE, TRUE);
};

TEST(Bool, TestPrinter) {
  EXPECT_EQ("true", ::testing::PrintToString(TRUE));
  EXPECT_EQ("false", ::testing::PrintToString(FALSE));
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
