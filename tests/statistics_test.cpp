#include <cmath>
#include <gtest/gtest.h>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "statistics.hpp"

using namespace bookdb;

class StatisticsTestSuite : public ::testing::Test {
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

TEST_F(StatisticsTestSuite, BuildAuthorHistogramFlatTest) {
    auto result = buildAuthorHistogramFlat(db);
    EXPECT_EQ(result.size(), 9);
    EXPECT_EQ(result.at("Aldous Huxley"), 1);
    EXPECT_EQ(result.at("George Orwell"), 2);

    auto empty_result = buildAuthorHistogramFlat(BookDatabase<>{});
    EXPECT_TRUE(empty_result.empty());
}

TEST_F(StatisticsTestSuite, CalculateGenreRatingsTest) {
    auto result = calculateGenreRatings(db.cbegin(), db.cend());

    EXPECT_EQ(result.size(), 2);
    EXPECT_NEAR(result.at(Genre::Fiction), 4.55, 0.001);
    EXPECT_NEAR(result.at(Genre::SciFi), 4.25, 0.001);

    auto empty_db = BookDatabase<>{};
    auto empty_result = calculateGenreRatings(empty_db.cbegin(), empty_db.cend());
    EXPECT_TRUE(empty_result.empty());
}

TEST_F(StatisticsTestSuite, CalculateAverageRatingTest) {
    auto result = calculateAverageRating(db);
    EXPECT_NEAR(result, 4.49, 0.001);

    auto empty_result = calculateAverageRating(BookDatabase<>{});
    EXPECT_EQ(empty_result, 0.0);
}

TEST_F(StatisticsTestSuite, SampleRandomBooksTest) {
    auto result = sampleRandomBooks(db, 2);
    EXPECT_EQ(result.size(), 2);

    BookDatabase<> one_book_db = {*db.begin()};
    auto one_result = sampleRandomBooks(one_book_db, 1);
    EXPECT_EQ(one_result[0].get().title, "1984");

    auto empty_result = sampleRandomBooks(db, 0);
    EXPECT_TRUE(empty_result.empty());
}

TEST_F(StatisticsTestSuite, GetTopNByTest) {
    auto result = getTopNBy(db, 2, comp::GreaterByRating{});
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].get().title, "The Hobbit");
    EXPECT_EQ(result[1].get().title, "To Kill a Mockingbird");

    auto empty_result = getTopNBy(db, 0, comp::GreaterByRating{});
    EXPECT_TRUE(empty_result.empty());
}