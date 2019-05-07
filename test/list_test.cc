#include "list.h"

#include <gtest/gtest.h>

#include "int.h"
#include "object.h"
#include "utils.h"

namespace sl {

TEST(ConsC, AllocAndDealloc) {
  const Object &o1 = Int::Val(5);
  const Object &o2 = Int::Val(10);
  const ConsC &pair = ConsC::Val(o1, o2);
  ASSERT_EQ(o1, pair.car());
  ASSERT_EQ(o2, pair.cdr());
}

TEST(ConsC, Equality) {
  const Object &o1 = Int::Val(5);
  const Object &o2 = Int::Val(10);
  const ConsC &pair_1 = ConsC::Val(o1, o2);
  const ConsC &pair_2 = ConsC::Val(o1, o2);
  EXPECT_TRUE(pair_1.IsEqual(pair_2));
  ASSERT_EQ(pair_1, pair_2);
  EXPECT_FALSE(pair_1.IsEqual(ConsC::Val(o1, Int::Val(7))));
  EXPECT_NE(pair_1, ConsC::Val(o1, Int::Val(7)));
}

TEST(Nil, Equality) {
  ASSERT_EQ(NIL, NIL);
  ASSERT_NE(NIL, Cons(Int::Val(1), NIL));
}

TEST(Nil, IsNil) {
  ASSERT_TRUE(IsNil(NIL));
  ASSERT_FALSE(IsNil(Cons(Int::Val(1), NIL)));
}

TEST(List, AllocAndDelloc) {
  const Object &o1 = Int::Val(5);
  const ConsC &head = ConsC::Val(o1, NIL);
  List list(head);
  EXPECT_NE(nullptr, &list);

  // Try initializer list
  // Requires Equality to be correct
  const List init_list({Int::Get(5)});
  EXPECT_EQ(list, init_list);
  const List init_list_long({Int::Get(5), Int::Get(6), Int::Get(7)});
  const List &expected =
      Cons(Int::Val(5), Cons(Int::Val(6), Cons(Int::Val(7), NIL)));
  EXPECT_EQ(expected, init_list_long);

  // Move Lists
  // const List init_list_2(List({Int::Get(5)}));
  // const List expected_2 = List{Int::Get(5)};
  // ASSERT_EQ(expected_2, init_list_2);
  // ASSERT_EQ(Int::Val(5), init_list_2.First());

  // Nested Lists
  const List init_list_3({new List({Int::Get(5)})});
  const List *tmp = new List({Int::Get(5)});
  const List expected_3({tmp});
  ASSERT_EQ(expected_3, init_list_3);
}

TEST(List, First) {
  const Object &o1 = Int::Val(5);
  const ConsC &head = ConsC::Val(o1, NIL);
  List list(head);
  ASSERT_EQ(o1, list.First());
  ASSERT_EQ(1, list.Count());
}

TEST(ListIterator, BeginAndEnd) {
  const List list({Int::Get(0), Int::Get(1), Int::Get(2)});
  List::ListIterator iter_begin = list.begin();
  List::ListIterator iter_end = list.end();
  ASSERT_EQ(List::ListIterator(NIL), iter_end);
  ASSERT_NE(List::ListIterator(NIL), iter_begin);
  for (int i = 0; i < 3; ++i, ++iter_begin) {
    ASSERT_TRUE(Int::Get(i)->IsEqual(*iter_begin));
  }
}

TEST(List, Str) {
  const Object &o1 = Int::Val(5);
  const ConsC &head = ConsC::Val(o1, NIL);
  List list(head);
  EXPECT_EQ("(5)", list.Str());
  EXPECT_EQ("()", NIL.Str());
  List list_2(ConsC::Val(Int::Val(1), list));
  EXPECT_EQ("(1 5)", list_2.Str());
}

TEST(List, PrintToString) {
  const Object &o1 = Int::Val(5);
  const ConsC &head = ConsC::Val(o1, NIL);
  List list(head);
  EXPECT_EQ("(5)", ::testing::PrintToString(list));
  List list_2({Int::Get(1), Int::Get(2)});
  EXPECT_EQ("(1 2)", ::testing::PrintToString(list_2));
  const ConsC temp(o1, NIL);
  new List(temp);
}

TEST(List, Equality) {
  const Object &o1 = Int::Val(5);
  const ConsC &head_1 = ConsC::Val(o1, NIL);
  List list_1(head_1);

  const ConsC &head_2 = ConsC::Val(o1, NIL);
  List list_2(head_2);
  ASSERT_NE(NIL, list_1);
  EXPECT_TRUE(list_1.IsEqual(list_2));
  ASSERT_EQ(list_1, list_2);

  const ConsC &head_3 = ConsC::Val(Int::Val(6), NIL);
  List list_3(head_3);
  EXPECT_FALSE(list_1.IsEqual(list_3));
  ASSERT_NE(list_1, list_3);

  // Equality with init lists
  ASSERT_EQ(List{Int::Get(1)}, List{Int::Get(1)});

  // Regression for bug:
  // Objects might be tested not equal because of static dispatch of operator==
  Object *obj = new List({Int::Get(1)});
  ASSERT_EQ(List{Int::Get(1)}, *obj);
}

TEST(List, Rest) {
  const Object &o1 = Int::Val(5);
  const ConsC &head = ConsC::Val(o1, NIL);
  const List &tail(head);
  const List &list = Cons(Int::Val(6), tail);
  EXPECT_TRUE(list.Rest().IsEqual(tail));
  ASSERT_EQ(tail, (list.Rest()));
  ASSERT_EQ(NIL, tail.Rest());
  ASSERT_EQ(Error(""), NIL.Rest());
}

TEST(List, Count) {
  const Object &o1 = Int::Val(5);
  const ConsC &head = ConsC::Val(o1, NIL);
  List list(head);
  EXPECT_EQ(1, list.Count());
  const List &new_list = Cons(Int::Val(6), list);
  EXPECT_EQ(2, new_list.Count());
}

}  // namespace sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
