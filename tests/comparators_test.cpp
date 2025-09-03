#include <gtest/gtest.h>

#include "book.hpp"
#include "comparators.hpp"

class ComparatorsTestSuite : public ::testing::Test {
protected:
    bookdb::Book book1{"Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10};
    bookdb::Book book2{"Ефремов", "Час Быка", 1970, bookdb::Genre::Fiction, 4.9, 11};
};

TEST_F(ComparatorsTestSuite, TestAuthorsCompare) {
    bookdb::comp::LessByAuthor less;
    bookdb::comp::GreaterByAuthor greater;

    EXPECT_TRUE(less(book1, book2));
    EXPECT_FALSE(greater(book1, book2));
}

TEST_F(ComparatorsTestSuite, TestTitleCompare) {
    bookdb::comp::LessByTitle less;
    bookdb::comp::GreaterByTitle greater;

    EXPECT_TRUE(less(book1, book2));
    EXPECT_FALSE(greater(book1, book2));
}

TEST_F(ComparatorsTestSuite, TestTitleRating) {
    bookdb::comp::LessByRating less;
    bookdb::comp::GreaterByRating greater;

    EXPECT_FALSE(less(book1, book2));
    EXPECT_TRUE(greater(book1, book2));
}

TEST_F(ComparatorsTestSuite, TestTitleReadCount) {
    bookdb::comp::LessByReadCount less;
    bookdb::comp::GreaterByReadCount greater;

    EXPECT_TRUE(less(book1, book2));
    EXPECT_FALSE(greater(book1, book2));
}