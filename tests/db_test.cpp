#include <deque>
#include <gtest/gtest.h>

#include "book.hpp"
#include "book_database.hpp"

class BookDbVectorTestSuite : public ::testing::Test {
protected:
    bookdb::BookDatabase<> db;
    bookdb::Book test_book1{"Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10};
    bookdb::Book test_book2{"Маркс", "Капитал", 1867, bookdb::Genre::NonFiction, 5.0, 10000000};

    void SetUp() override {
        db.emplace_back("Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10);
        db.emplace_back("Ефремов", "Час Быка", 1970, bookdb::Genre::Fiction, 4.9, 11);
        db.emplace_back("author1", "title_1_1", 2022, bookdb::Genre::Mystery, 3.9, 110);
        db.emplace_back("author1", "title_1_2", 2025, bookdb::Genre::Mystery, 4.2, 210);
        db.emplace_back("author2", "title_2_1", 2025, bookdb::Genre::Biography, 5.0, 23210);
    }
};

class BookDbDequeTestSuite : public ::testing::Test {
protected:
    bookdb::BookDatabase<std::deque<bookdb::Book>> db;
    bookdb::Book test_book1{"Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10};
    bookdb::Book test_book2{"Маркс", "Капитал", 1867, bookdb::Genre::NonFiction, 5.0, 10000000};

    void SetUp() override {
        db.emplace_back("Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10);
        db.emplace_back("Ефремов", "Час Быка", 1970, bookdb::Genre::Fiction, 4.9, 11);
        db.emplace_back("author1", "title_1_1", 2022, bookdb::Genre::Mystery, 3.9, 110);
        db.emplace_back("author1", "title_1_2", 2025, bookdb::Genre::Mystery, 4.2, 210);
        db.emplace_back("author2", "title_2_1", 2025, bookdb::Genre::Biography, 5.0, 23210);
    }
};

TEST(BookDbTest, BasicTest) {
    bookdb::BookDatabase<> db;
    EXPECT_EQ(db.size(), 0);
    db.emplace_back("Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10);
    db.push_back(bookdb::Book{"Ефремов", "Час Быка", 1970, bookdb::Genre::Fiction, 4.9, 11});
    EXPECT_EQ(db.size(), 2);
    auto books = db.get_books();
    EXPECT_EQ(books.size(), 2);

    EXPECT_EQ(books[0].author, "Булгаков");
    EXPECT_EQ(books[0].title, "Мастер и Маргарита");
    EXPECT_EQ(books[0].year, 1966);
    EXPECT_EQ(books[0].genre, bookdb::Genre::Mystery);
    EXPECT_EQ(books[0].rating, 5);
    EXPECT_EQ(books[0].read_count, 10);

    EXPECT_EQ(books[1].author, "Ефремов");
    EXPECT_EQ(books[1].title, "Час Быка");
    EXPECT_EQ(books[1].year, 1970);
    EXPECT_EQ(books[1].genre, bookdb::Genre::Fiction);
    EXPECT_EQ(books[1].rating, 4.9);
    EXPECT_EQ(books[1].read_count, 11);
}

TEST_F(BookDbVectorTestSuite, VectorBasicTest) {
    EXPECT_EQ(db.size(), 5);
    db.push_back(test_book2);
    EXPECT_EQ(db.size(), 6);

    auto books = db.get_books();
    EXPECT_EQ(books.back().title, test_book2.title);
    EXPECT_EQ(books.back().author, test_book2.author);
    EXPECT_EQ(books.back().year, test_book2.year);
    EXPECT_EQ(books.back().genre, test_book2.genre);
    EXPECT_EQ(books.back().rating, test_book2.rating);
    EXPECT_EQ(books.back().read_count, test_book2.read_count);

    auto authors = db.get_authors();
    EXPECT_TRUE(authors.contains("Булгаков"));
    EXPECT_TRUE(authors.contains("Маркс"));
    EXPECT_FALSE(authors.contains("Энгельс"));

    auto book = *(++db.begin());
    EXPECT_EQ(book.author, books[1].author);
    EXPECT_EQ(book.title, books[1].title);
}

TEST_F(BookDbDequeTestSuite, DequeBasicTest) {
    EXPECT_EQ(db.size(), 5);
    db.push_back(test_book2);
    EXPECT_EQ(db.size(), 6);

    auto books = db.get_books();
    EXPECT_EQ(books.back().title, test_book2.title);
    EXPECT_EQ(books.back().author, test_book2.author);
    EXPECT_EQ(books.back().year, test_book2.year);
    EXPECT_EQ(books.back().genre, test_book2.genre);
    EXPECT_EQ(books.back().rating, test_book2.rating);
    EXPECT_EQ(books.back().read_count, test_book2.read_count);

    auto authors = db.get_authors();
    EXPECT_TRUE(authors.contains("Булгаков"));
    EXPECT_TRUE(authors.contains("Маркс"));
    EXPECT_FALSE(authors.contains("Энгельс"));

    auto book = *(++db.begin());
    EXPECT_EQ(book.author, books[1].author);
    EXPECT_EQ(book.title, books[1].title);
}

TEST_F(BookDbVectorTestSuite, InitializerListTest) {
    auto it = db.begin();
    bookdb::BookDatabase<> db_test{*(it), *(++it), *(++it)};
    EXPECT_EQ(db_test.size(), 3);
    auto books = db.get_books();
    auto books_test = db_test.get_books();
    EXPECT_EQ(books[0].title, books_test[0].title);
    EXPECT_EQ(books[1].title, books_test[1].title);
    EXPECT_EQ(books[2].title, books_test[2].title);
    EXPECT_NE(books.size(), books_test.size());
}