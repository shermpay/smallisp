#include <gtest/gtest.h>

#include "list.h"
#include "objects.h"

namespace sl {

TEST(ConsC, AllocAndDealloc) {
  Object *o1 = Int::Get(5);
  Object *o2 = Int::Get(10);
  ConsC *pair = new ConsC(o1, o2);
  ASSERT_EQ(o1, pair->car());
  ASSERT_EQ(o2, pair->cdr());
}

TEST(ConsC, Equality) {
  Object *o1 = Int::Get(5);
  Object *o2 = Int::Get(10);
  ConsC *pair_1 = new ConsC(o1, o2);
  ConsC *pair_2 = new ConsC(o1, o2);
  EXPECT_TRUE(pair_1->IsEqual(*pair_2));
  ASSERT_EQ(*pair_1, *pair_2);
  EXPECT_FALSE(pair_1->IsEqual(ConsC(o1, Int::Get(7))));
  EXPECT_NE(pair_1, pair_2);
}

TEST(Nil, Equality) {
  ASSERT_EQ(kNil, kNil);
  ASSERT_NE(*kNil, *Cons(Int::Get(1), kNil));
  ASSERT_EQ(nullptr, kNil->head());
}

TEST(Nil, IsNil) {
  ASSERT_TRUE(IsNil(kNil));
  ASSERT_FALSE(IsNil(Cons(Int::Get(1), kNil)));
}

TEST(List, AllocAndDelloc) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
  EXPECT_NE(nullptr, list);

  // Try initializer list
  // Requires Equality to be correct
  const List init_list({Int::Get(5)});
  EXPECT_EQ(*list, init_list);
  const List init_list_long({Int::Get(5), Int::Get(6), Int::Get(7)});
  const List *expected =
      Cons(Int::Get(5), Cons(Int::Get(6), Cons(Int::Get(7), kNil)));
  EXPECT_EQ(*expected, init_list_long);

  // Move Lists
  const List init_list_2(List({Int::Get(5)}));
  const List expected_2 = List{Int::Get(5)};
  ASSERT_EQ(expected_2, init_list_2);
  ASSERT_EQ(Int::Get(5), init_list_2.First());

  // Nested Lists
  const List init_list_3({new List({Int::Get(5)})});
  const List *tmp = new List({Int::Get(5)});
  const List expected_3({tmp});
  ASSERT_EQ(expected_3, init_list_3);
}

TEST(List, First) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
  ASSERT_EQ(o1, list->First());
  ASSERT_EQ(1, list->Count());
}

TEST(ListIterator, BeginAndEnd) {
  const List list({Int::Get(0), Int::Get(1), Int::Get(2)});
  List::ListIterator iter_begin = list.begin();
  List::ListIterator iter_end = list.end();
  ASSERT_EQ(List::ListIterator(*kNil), iter_end);
  ASSERT_NE(List::ListIterator(*kNil), iter_begin);
  for (int i = 0; i < 3; ++i, ++iter_begin) {
    ASSERT_TRUE(Int::Get(i)->IsEqual(*iter_begin));
  }
}

TEST(List, Str) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
  EXPECT_EQ("(5)", list->Str());
  EXPECT_EQ("()", kNil->Str());
  List list_2(new ConsC(Int::Get(1), list));
  EXPECT_EQ("(1 5)", list_2.Str());
}

TEST(List, PrintToString) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
  EXPECT_EQ("(5)", ::testing::PrintToString(*list));
  List list_2({Int::Get(1), Int::Get(2)});
  EXPECT_EQ("(1 2)", ::testing::PrintToString(list_2));
}

TEST(List, Equality) {
  Object *o1 = Int::Get(5);
  const ConsC *head_1 = new ConsC(o1, kNil);
  List *list_1 = new List(head_1);

  const ConsC *head_2 = new ConsC(o1, kNil);
  List *list_2 = new List(head_2);
  ASSERT_NE(kNil, list_1);
  EXPECT_TRUE(list_1->IsEqual(*list_2));
  ASSERT_EQ(*list_1, *list_2);

  const ConsC *head_3 = new ConsC(Int::Get(6), kNil);
  List *list_3 = new List(head_3);
  EXPECT_FALSE(list_1->IsEqual(*list_3));
  ASSERT_NE(*list_1, *list_3);
}

TEST(List, Rest) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  const List *tail = new List(head);
  const List *list = Cons(Int::Get(6), tail);
  EXPECT_TRUE(list->Rest()->IsEqual(*tail));
  ASSERT_EQ(*tail, *(list->Rest()));
}

TEST(List, Count) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
  EXPECT_EQ(1, list->Count());
  const List *new_list = Cons(Int::Get(6), list);
  EXPECT_EQ(2, new_list->Count());
}

}  // namespace sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
