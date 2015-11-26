#include "objects.h"
#include <gtest/gtest.h>
#include <unordered_set>
#include <unordered_map>

namespace sl {

////////////////////////
// Generic Operations //
////////////////////////
TEST(Hashing, ObjectHashFn) {
  HashFn<Object> hash_fn;
  EXPECT_EQ(hash_fn(Symbol::Val("a")), hash_fn(Symbol::Val("a")));
}

TEST(Hashing, ObjectInHashSet) {
  std::unordered_set<std::reference_wrapper<const Object>, HashFn<Object>>
      obj_set;
  obj_set.insert(Int::Val(1));
  obj_set.insert(Symbol::Val("foo"));
  EXPECT_EQ(2, obj_set.size());
  std::unordered_set<std::reference_wrapper<const Symbol>, HashFn<Object>>
      sym_set;
  sym_set.insert(Symbol::Val("foo"));
  sym_set.insert(Symbol::Val("bar"));
  EXPECT_EQ(2, sym_set.size());
  std::unordered_map<Symbol, int, HashFn<Object>> sym_map{
      {Symbol::Val("bar"), 1}};
  sym_map.insert({Symbol::Val("foo"), 1});
  sym_map.insert({Symbol::Val("bar"), 1});
  EXPECT_EQ(3, sym_map.size());
}

/////////////////////
// Type Operations //
/////////////////////
TEST(Type, IsType) {
  ASSERT_TRUE(IsType<Int>(Int::Val(0)));
  ASSERT_TRUE(IsType<Symbol>(Symbol::Val("foo")));
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
