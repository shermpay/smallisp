#include "reader.h"

#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

namespace sl {

namespace {

TEST(Reader, ReadWhitespace) {
  std::istringstream sstream("       \n   \t ");
  Reader reader(sstream);
  reader.ReadWhitespace();
  EXPECT_EQ(Reader::kStartLinum + 1, reader.linum());
  EXPECT_EQ(Reader::kStartColnum + 5, reader.colnum());

  std::istringstream sstream_2("\n\n   1  ");
  Reader reader_2(sstream_2);
  reader_2.ReadWhitespace();
  EXPECT_EQ(Reader::kStartLinum + 2, reader_2.linum());
  EXPECT_EQ(Reader::kStartColnum + 3, reader_2.colnum());
  reader_2.ReadWhitespace();
  EXPECT_EQ(Reader::kStartLinum + 2, reader_2.linum());
  EXPECT_EQ(Reader::kStartColnum + 3, reader_2.colnum());
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

TEST(Reader, ReadSexp) {
  std::stringstream sstream("(foo)");
  Reader reader(sstream);
  const List *sexp = reader.ReadSexp();
  ASSERT_EQ(List({Symbol::Get("foo")}), *sexp);

  std::stringstream sstream_2("(foo 1 2)");
  Reader reader_2(sstream_2);
  sexp = reader_2.ReadSexp();
  ASSERT_EQ(List({Symbol::Get("foo"), Int::Get(1), Int::Get(2)}), *sexp);

  std::stringstream sstream_3("(foo (bar 1 2) 3)");
  Reader reader_3(sstream_3);
  sexp = reader_3.ReadSexp();
  ASSERT_EQ(List({Symbol::Get("foo"),
                  new List({Symbol::Get("bar"), Int::Get(1), Int::Get(2)}),
                  Int::Get(3)}),
            *sexp);
}
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
