#include <gtest/gtest.h>

#include "book.hpp"
#include "heterogeneous_lookup.hpp"

class HeterogeneousTestSuite : public ::testing::Test {
protected:
    bookdb::Book book1{"Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10};
    bookdb::Book book2{"Ефремов", "Час Быка", 1970, bookdb::Genre::Fiction, 4.9, 11};
    bookdb::Book book3{"Неевремов", "Час Быка", 2050, bookdb::Genre::NonFiction, 1.9, 1};
};

TEST_F(HeterogeneousTestSuite, TestLess) {
    bookdb::TransparentStringLess less;

    EXPECT_TRUE(less(book1, book2));
    EXPECT_TRUE(less(book1, "Час Быка"));
    EXPECT_TRUE(less("Азбука", book1));
    EXPECT_TRUE(less("Азбука", "Час Быка"));
}

TEST_F(HeterogeneousTestSuite, TestEqual) {
    bookdb::TransparentStringEqual equal;

    EXPECT_FALSE(equal(book1, book2));
    EXPECT_FALSE(equal(book1, "Час Быка"));
    EXPECT_FALSE(equal("Час Быка", book1));
    EXPECT_FALSE(equal("Мастер и Маргарита", "Час Быка"));

    EXPECT_TRUE(equal(book1, book1));
    EXPECT_TRUE(equal("Мастер и Маргарита", book1));
    EXPECT_TRUE(equal(book1, "Мастер и Маргарита"));
    EXPECT_TRUE(equal("Мастер и Маргарита", "Мастер и Маргарита"));
    EXPECT_TRUE(equal(book2, book3));
}

TEST_F(HeterogeneousTestSuite, TestHash) {
    bookdb::TransparentStringHash hash;

    EXPECT_NE(hash(book1), hash(book2));
    EXPECT_NE(hash(book1), hash("Час Быка"));
    EXPECT_NE(hash("Час Быка"), hash(book1));
    EXPECT_NE(hash("Мастер и Маргарита"), hash("Час Быка"));

    EXPECT_EQ(hash(book1), hash(book1));
    EXPECT_EQ(hash("Мастер и Маргарита"), hash(book1));
    EXPECT_EQ(hash(book1), hash("Мастер и Маргарита"));
    EXPECT_EQ(hash("Мастер и Маргарита"), hash("Мастер и Маргарита"));
    EXPECT_EQ(hash(book2), hash(book3));
}