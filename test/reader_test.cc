#include "reader.h"

#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include "int.h"
#include "list.h"
#include "symbol.h"

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
  const Object &neg_one = reader.ReadInt();
  ASSERT_EQ(Int::Val(-1), neg_one);
  reader.ReadWhitespace();
  const Object &two = reader.ReadInt();
  ASSERT_EQ(Int::Val(2), two);
  reader.ReadWhitespace();
  const Object &short_max = reader.ReadInt();
  ASSERT_EQ(Int::Val(65536), short_max);
  reader.ReadWhitespace();
  const Object &zero = reader.ReadInt();
  ASSERT_EQ(Int::Val(0), zero);
}

TEST(Reader, ReadSymbol) {
  std::istringstream sstream("foo bar-1 -baz");
  Reader reader(sstream);
  reader.ReadWhitespace();
  const Object &foo = reader.ReadSymbol();
  ASSERT_EQ(Symbol::Val("foo"), foo);
  reader.ReadWhitespace();
  const Object &bar = reader.ReadSymbol();
  ASSERT_EQ(Symbol::Val("bar-1"), bar);
  reader.ReadWhitespace();
  const Object &baz = reader.ReadSymbol();
  ASSERT_EQ(Symbol::Val("-baz"), baz);
}

TEST(Reader, ReadSexp) {
  std::stringstream sstream("(foo)");
  Reader reader(sstream);
  const Object &sexp = reader.ReadSexp();
  const List expected{Symbol::Get("foo")};
  ASSERT_EQ(expected, sexp);

  std::stringstream sstream_2("(foo 1 2)");
  Reader reader_2(sstream_2);
  const Object &sexp_2 = reader_2.ReadSexp();
  ASSERT_EQ(List({Symbol::Get("foo"), Int::Get(1), Int::Get(2)}), sexp_2);

  std::stringstream sstream_3("(foo (bar 1 2) 3)");
  Reader reader_3(sstream_3);
  const Object &sexp_3 = reader_3.ReadSexp();
  ASSERT_EQ(List({Symbol::Get("foo"),
                  new List({Symbol::Get("bar"), Int::Get(1), Int::Get(2)}),
                  Int::Get(3)}),
            sexp_3);

  std::stringstream sstream_4("(foo (bar 1 2)");
  Reader reader_4(sstream_4);
  const Object &sexp_4 = reader_4.ReadSexp();
  ASSERT_EQ(Error(""), sexp_4);

  std::stringstream ss_empty("()");
  Reader reader_empty(ss_empty);
  const Object &sexp_5 = reader_empty.ReadSexp();
  ASSERT_EQ(NIL, sexp_5);
}

TEST(Reader, ReadExpr) {
  std::stringstream sstream("(foo)1");
  Reader reader(sstream);
  const Object &sexp = reader.ReadExpr();
  ASSERT_TRUE(IsType<List>(sexp));
  ASSERT_TRUE(sexp.IsEqual(List({Symbol::Get("foo")})));

  const Object &num = reader.ReadExpr();
  ASSERT_TRUE(IsType<Int>(num));
  ASSERT_TRUE(num.IsEqual(Int::Val(1)));
}

TEST(Reader, ReadExprList) {
  std::stringstream sstream("(foo) 1 2");
  Reader reader(sstream);
  const std::vector<const Object *> exprs = reader.ReadExprList();
  ASSERT_TRUE(exprs[0]->IsEqual(List({Symbol::Get("foo")})));
  ASSERT_TRUE(exprs[1]->IsEqual(Int::Val(1)));
  ASSERT_TRUE(exprs[2]->IsEqual(Int::Val(2)));
}

}  // anonymous
}  // sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
