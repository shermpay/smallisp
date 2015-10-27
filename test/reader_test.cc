#include "reader.h"

#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

namespace sl {

TEST(Reader, ReadWhitespace) {
  std::istringstream sstream("       \n   \t ");
  Reader reader(sstream);
  reader.ReadWhitespace();
  EXPECT_EQ(Reader::kStartLinum + 1, reader.linum());
  EXPECT_EQ(Reader::kStartColnum + 5, reader.colnum());
}

TEST(Reader, ReadInt) {
  std::istringstream sstream("-1 2 65536 0");
  Reader reader(sstream);
  reader.ReadWhitespace();
  EXPECT_EQ(Reader::kStartLinum, reader.linum());
  EXPECT_EQ(Reader::kStartColnum, reader.colnum());
  const Int *neg_one = reader.ReadInt();
  ASSERT_EQ(*Int::Get(-1), *neg_one);
  reader.ReadWhitespace();
  const Int *two = reader.ReadInt();
  ASSERT_EQ(*Int::Get(2), *two);
  reader.ReadWhitespace();
  const Int *short_max = reader.ReadInt();
  ASSERT_EQ(*Int::Get(65536), *short_max);
  reader.ReadWhitespace();
  const Int *zero = reader.ReadInt();
  ASSERT_EQ(*Int::Get(0), *zero);
}

TEST(Reader, ReadSymbol) {
  std::istringstream sstream("foo bar-1 -baz");
  Reader reader(sstream);
  reader.ReadWhitespace();
  const Symbol *foo = reader.ReadSymbol();
  ASSERT_EQ(*Symbol::Get("foo"), *foo);
  reader.ReadWhitespace();
  const Symbol *bar = reader.ReadSymbol();
  ASSERT_EQ(*Symbol::Get("bar-1"), *bar);
  reader.ReadWhitespace();
  const Symbol *baz = reader.ReadSymbol();
  ASSERT_EQ(*Symbol::Get("-baz"), *baz);
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
