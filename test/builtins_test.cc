// This is a test to ensure that we were able to compile and link with our
// testing framework.

#include "builtins.h"
#include <gtest/gtest.h>

namespace sl {
namespace builtins {
TEST(BuiltinFunction, Add) {
  ASSERT_EQ(Int::Val(2), *Add()(List{Int::Get(1), Int::Get(1)}));
}

TEST(BuiltinFunction, Sub) {
  ASSERT_EQ(Int::Val(-1), *Sub()(List{Int::Get(0), Int::Get(1)}));
}

TEST(BuiltinFunction, Mul) {
  ASSERT_EQ(Int::Val(12), *Mul()(List{Int::Get(3), Int::Get(4)}));
}

TEST(BuiltinFunction, Div) {
  ASSERT_EQ(Int::Val(2), *Div()(List{Int::Get(8), Int::Get(4)}));
}

TEST(BuiltinFunction, Eq) {
  ASSERT_EQ(FALSE, *Eq()(List{Int::Get(8), Int::Get(4)}));
  ASSERT_EQ(TRUE, *Eq()(List{Int::Get(4), Int::Get(4)}));
}

TEST(BuiltinFunction, Ne) {
  ASSERT_EQ(TRUE, *Ne()(List{Int::Get(8), Int::Get(4)}));
  ASSERT_EQ(FALSE, *Ne()(List{Int::Get(4), Int::Get(4)}));
}

}  // namespace builtins
}  // namespace sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
