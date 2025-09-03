#include <format>
#include <gtest/gtest.h>

#include "book.hpp"

TEST(BookTest, TestStringToGenreAllGenres) {
    EXPECT_EQ(bookdb::GenreFromString("Fiction"), bookdb::Genre::Fiction);
    EXPECT_EQ(bookdb::GenreFromString("NonFiction"), bookdb::Genre::NonFiction);
    EXPECT_EQ(bookdb::GenreFromString("SciFi"), bookdb::Genre::SciFi);
    EXPECT_EQ(bookdb::GenreFromString("Biography"), bookdb::Genre::Biography);
    EXPECT_EQ(bookdb::GenreFromString("Mystery"), bookdb::Genre::Mystery);
    EXPECT_EQ(bookdb::GenreFromString("Unknown"), bookdb::Genre::Unknown);
}

TEST(BookTest, TestStringToGenreWrongCases) {
    EXPECT_EQ(bookdb::GenreFromString("fiction"), bookdb::Genre::Unknown);
    EXPECT_EQ(bookdb::GenreFromString(""), bookdb::Genre::Unknown);
    EXPECT_EQ(bookdb::GenreFromString(" SciFi"), bookdb::Genre::Unknown);
    EXPECT_EQ(bookdb::GenreFromString("unknown"), bookdb::Genre::Unknown);
    EXPECT_EQ(bookdb::GenreFromString("Mystery "), bookdb::Genre::Unknown);
    EXPECT_EQ(bookdb::GenreFromString("Biog raphy"), bookdb::Genre::Unknown);
}

TEST(BookTest, TestStringToGenreConstexprEvaluation) {
    constexpr bookdb::Genre fiction = bookdb::GenreFromString("Fiction");
    constexpr bookdb::Genre unknown = bookdb::GenreFromString("");

    static_assert(fiction == bookdb::Genre::Fiction);
    static_assert(unknown == bookdb::Genre::Unknown);

    EXPECT_EQ(fiction, bookdb::Genre::Fiction);
    EXPECT_EQ(unknown, bookdb::Genre::Unknown);
}

TEST(BookTest, TestConstructors) {
    bookdb::Book book_from_genre{"Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10};
    EXPECT_EQ(book_from_genre.genre, bookdb::Genre::Mystery);

    bookdb::Book book_from_genre_string{"Булгаков", "Мастер и Маргарита", 1966, "Mystery", 5.0, 10};
    EXPECT_EQ(book_from_genre_string.genre, bookdb::Genre::Mystery);

    bookdb::Book book_from_another_genre{"Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Biography, 5.0, 10};
    EXPECT_EQ(book_from_another_genre.genre, bookdb::Genre::Biography);

    EXPECT_NE(book_from_genre_string, book_from_another_genre);
}

class BookTestSuite : public ::testing::Test {
protected:
    bookdb::Book book1{"Булгаков", "Мастер и Маргарита", 1966, bookdb::Genre::Mystery, 5.0, 10};
    bookdb::Book book2{"Ефремов", "Час Быка", 1970, bookdb::Genre::Fiction, 4.9, 11};
};

TEST_F(BookTestSuite, TestFormatter) {
    auto short_result = "[Булгаков, 1966]";
    auto long_result = "Author: Булгаков, Title: Мастер и Маргарита, Year: 1966, Genre: Mystery, Rating: 5, Reads: 10";
    EXPECT_EQ(std::format("{}", book1), long_result);
    EXPECT_EQ(std::format("{0:}", book1), long_result);
    EXPECT_EQ(std::format("{0:#}", book1), short_result);
}