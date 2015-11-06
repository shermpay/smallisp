#include "objects.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

namespace sl {

TEST(ObjectAllocation, IntAllocation) {
  Int *integer = Int::Get(5);
  EXPECT_EQ(5, integer->value());
}

TEST(ObjectAllocation, SymbolAllocation) {
  Symbol *sym_foo = Symbol::Get("Foo");
  EXPECT_EQ("Foo", sym_foo->name());
  Symbol *sym_bar = Symbol::Get("Bar");
  EXPECT_EQ("Bar", sym_bar->name());
  Symbol *sym_foo_ptr = Symbol::Get("Foo");
  EXPECT_EQ("Foo", sym_foo_ptr->name());
  EXPECT_EQ(sym_foo, sym_foo_ptr);
}

TEST(ObjectEquality, IntEquality) {
  Int *integer = Int::Get(5);
  EXPECT_EQ(Int::Get(5), integer);
  EXPECT_NE(Int::Get(6), integer);
};

TEST(ObjectEquality, SymbolEquality) {
  Symbol *sym_foo = Symbol::Get("foo");
  EXPECT_EQ(Symbol::Get("foo"), sym_foo);
  EXPECT_NE(Symbol::Get("bar"), sym_foo);
  EXPECT_EQ(*Symbol::Get("foo"), *sym_foo);
  EXPECT_NE(*Symbol::Get("bar"), *sym_foo);
}

TEST(ObjectTestPrinter, IntTestPrinter) {
  Int *integer = Int::Get(5);
  EXPECT_EQ("5", ::testing::PrintToString(*integer));
}

TEST(ObjectTestPrinter, SymbolTestPrinter) {
  const Symbol *foo = Symbol::Get("foo");
  EXPECT_EQ("foo", ::testing::PrintToString(*foo));
}

TEST(Hashing, IntHashcode) {
  EXPECT_NE(Int::Get(0)->Hashcode(), Int::Get(1)->Hashcode());
  EXPECT_EQ(Int::Get(0)->Hashcode(), Int::Get(0)->Hashcode());
}

TEST(Hashing, SymbolHashcode) {
  EXPECT_NE(Symbol::Get("a")->Hashcode(), Symbol::Get("A")->Hashcode());
  EXPECT_EQ(Symbol::Get("a")->Hashcode(), Symbol::Get("a")->Hashcode());
}

TEST(Hashing, ObjectHashFn) {
  ObjectHashFn<Object *> hash_fn;
  EXPECT_EQ(hash_fn(Symbol::Get("a")), hash_fn(Symbol::Get("a")));
}

TEST(Hashing, ObjectInHashSet) {
  std::unordered_set<const Object *> obj_set;
  obj_set.insert(Int::Get(1));
  obj_set.insert(Symbol::Get("foo"));
  EXPECT_EQ(2, obj_set.size());
  std::unordered_set<const Symbol *> sym_set;
  sym_set.insert(Symbol::Get("foo"));
  sym_set.insert(Symbol::Get("bar"));
  EXPECT_EQ(2, sym_set.size());
  std::unordered_map<const Symbol *, int> sym_map{{Symbol::Get("bar"), 1}};
  sym_map.insert({Symbol::Get("foo"), 1});
  sym_map.insert({Symbol::Get("bar"), 1});
  EXPECT_EQ(2, sym_map.size());
}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
