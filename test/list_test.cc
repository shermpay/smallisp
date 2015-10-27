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

TEST(List, AllocAndDelloc) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
}

TEST(List, First) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
  ASSERT_EQ(o1, list->First());
  ASSERT_EQ(1, list->Count());
}

TEST(List, Str) {
  Object *o1 = Int::Get(5);
  const ConsC *head = new ConsC(o1, kNil);
  List *list = new List(head);
  EXPECT_EQ("(5)", list->Str());
}

TEST(List, Equality) {
  ASSERT_EQ(kNil, kNil);
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

} // namespace sl

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
