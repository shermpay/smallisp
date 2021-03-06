#include "array.h"

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include "int.h"
#include "list.h"

namespace sl {
TEST(Array, BasicOperations) {
  const Object *vals[] = {Int::Get(0), Int::Get(1)};
  Array arr(2, vals);
  ASSERT_TRUE(IsType<Array>(arr));
  ASSERT_EQ(2, arr.size());
  ASSERT_EQ(Int::Val(0), arr[0]);
  ASSERT_EQ(Int::Val(1), arr[1]);
  ASSERT_TRUE(IsType<Error>(arr[2]));
}

TEST(Array, InitList) {
  Array arr({Int::Get(1), Int::Get(2)});
  ASSERT_EQ(2, arr.size());
  ASSERT_EQ(Int::Val(1), arr[0]);
  ASSERT_EQ(Int::Val(2), arr[1]);
}

TEST(Array, EdgeCaseInitialization) {
  Array arr2({});
  ASSERT_EQ(0, arr2.size());
}

TEST(Array, Equality) {
  ASSERT_EQ(Array({Int::Get(0)}), Array({Int::Get(0)}));
  ASSERT_NE(Array({Int::Get(0)}), Array({Int::Get(1)}));
  ASSERT_NE(Array({Int::Get(0)}), Array({Int::Get(0), Int::Get(0)}));
  ASSERT_TRUE(Array({Int::Get(0)}).IsEqual(Array({Int::Get(0)})));
}

TEST(Array, Indexing) {
  Array arr({Int::Get(1), Int::Get(2)});
  ASSERT_EQ(Int::Val(1), arr[0]);
  ASSERT_TRUE(IsType<Error>(arr[-1]));
}

TEST(Array, Str) {
  EXPECT_EQ("[1 2]", Array({Int::Get(1), Int::Get(2)}).Str());
  EXPECT_EQ("[]", Array({}).Str());
  EXPECT_EQ("[0]", Array({Int::Get(0)}).Str());
}

TEST(Array, MoveCtor) {
  Array arr({Int::Get(1), Int::Get(2)});
  ASSERT_EQ(arr, Array(arr));
}

TEST(Array, FromList) {
  List list({Int::Get(1), Int::Get(42)});
  const Array &expected({Int::Get(1), Int::Get(42)});
  const Array &t1 = Array::FromList(list);
  ASSERT_EQ(expected, t1);
  ASSERT_EQ(expected, Array(list));
}

}  // namespace sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
