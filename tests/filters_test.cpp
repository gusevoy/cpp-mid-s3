#include <cmath>
#include <gtest/gtest.h>

#include "book.hpp"
#include "book_database.hpp"
#include "filters.hpp"

using namespace bookdb;

class FiltersTestSuite : public ::testing::Test {
protected:
    bookdb::BookDatabase<> db;

    void SetUp() override {
        db.emplace_back("George Orwell", "1984", 1949, Genre::SciFi, 4., 190);
        db.emplace_back("George Orwell", "Animal Farm", 1945, Genre::Fiction, 4.4, 143);
        db.emplace_back("F. Scott Fitzgerald", "The Great Gatsby", 1925, Genre::Fiction, 4.5, 120);
        db.emplace_back("Harper Lee", "To Kill a Mockingbird", 1960, Genre::Fiction, 4.8, 156);
        db.emplace_back("Jane Austen", "Pride and Prejudice", 1813, Genre::Fiction, 4.7, 178);
        db.emplace_back("J.D. Salinger", "The Catcher in the Rye", 1951, Genre::Fiction, 4.3, 112);
        db.emplace_back("Aldous Huxley", "Brave New World", 1932, Genre::SciFi, 4.5, 98);
        db.emplace_back("Charlotte Brontë", "Jane Eyre", 1847, Genre::Fiction, 4.6, 110);
        db.emplace_back("J.R.R. Tolkien", "The Hobbit", 1937, Genre::Fiction, 4.9, 203);
        db.emplace_back("William Golding", "Lord of the Flies", 1954, Genre::Fiction, 4.2, 89);
    }
};

TEST_F(FiltersTestSuite, YearBetweenTest) {
    auto result = std::find_if(db.cbegin(), db.cend(), YearBetween(1800, 1900));
    EXPECT_NE(result, db.cend());
    EXPECT_EQ(result->title, "Pride and Prejudice");

    auto empty_result = std::find_if(db.cbegin(), db.cend(), YearBetween(1000, 1100));
    EXPECT_EQ(empty_result, db.cend());
}

TEST_F(FiltersTestSuite, RatingAboveTest) {
    auto result = std::find_if(db.cbegin(), db.cend(), RatingAbove(4.9));
    EXPECT_NE(result, db.cend());
    EXPECT_EQ(result->title, "The Hobbit");

    auto empty_result = std::find_if(db.cbegin(), db.cend(), RatingAbove(10));
    EXPECT_EQ(empty_result, db.cend());
}

TEST_F(FiltersTestSuite, GenreIsTest) {
    auto result = std::find_if(db.cbegin(), db.cend(), GenreIs(Genre::Fiction));
    EXPECT_NE(result, db.cend());
    EXPECT_EQ(result->title, "Animal Farm");

    auto empty_result = std::find_if(db.cbegin(), db.cend(), GenreIs(Genre::Unknown));
    EXPECT_EQ(empty_result, db.cend());
}

TEST_F(FiltersTestSuite, AnyOfTest) {
    auto result = std::find_if(db.cbegin(), db.cend(), any_of(GenreIs(Genre::Fiction), YearBetween(1940, 1950)));
    EXPECT_NE(result, db.cend());
    EXPECT_EQ(result->title, "1984");

    auto result2 = std::find_if(db.cbegin(), db.cend(), any_of(GenreIs(Genre::Fiction), YearBetween(1940, 1945)));
    EXPECT_NE(result2, db.cend());
    EXPECT_EQ(result2->title, "Animal Farm");

    auto empty_result = std::find_if(db.cbegin(), db.cend(), any_of(GenreIs(Genre::Unknown), YearBetween(1700, 1750)));
    EXPECT_EQ(empty_result, db.cend());
}

TEST_F(FiltersTestSuite, AllOfTest) {
    auto result = std::find_if(db.cbegin(), db.cend(), all_of(GenreIs(Genre::Fiction), YearBetween(1940, 1950)));
    EXPECT_NE(result, db.cend());
    EXPECT_EQ(result->title, "Animal Farm");

    auto result2 = std::find_if(db.cbegin(), db.cend(), all_of(GenreIs(Genre::Fiction), YearBetween(1950, 1960)));
    EXPECT_NE(result2, db.cend());
    EXPECT_EQ(result2->title, "To Kill a Mockingbird");

    auto empty_result = std::find_if(db.cbegin(), db.cend(), all_of(GenreIs(Genre::Unknown), YearBetween(1700, 2000)));
    EXPECT_EQ(empty_result, db.cend());
}

TEST_F(FiltersTestSuite, FilterBooksTest) {
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    EXPECT_EQ(filtered.size(), 4);

    auto empty_result = filterBooks(db.begin(), db.end(), all_of(YearBetween(1700, 1800), RatingAbove(4.5)));
    EXPECT_TRUE(empty_result.empty());
}