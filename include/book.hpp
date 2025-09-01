#pragma once

#include <array>
#include <format>
#include <iterator>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr std::array genreMappings{std::pair{Genre::Fiction, "Fiction"}, std::pair{Genre::NonFiction, "NonFiction"},
                                   std::pair{Genre::SciFi, "SciFi"},     std::pair{Genre::Biography, "Biography"},
                                   std::pair{Genre::Mystery, "Mystery"}, std::pair{Genre::Unknown, "Unknown"}};

constexpr Genre GenreFromString(std::string_view s) {
    const auto it =
        std::find_if(genreMappings.begin(), genreMappings.end(), [s](const auto& pair) { return pair.second == s; });
    return (it != genreMappings.end()) ? it->first : Genre::Unknown;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    // Ваш код для конструкторов здесь
    constexpr Book(std::string_view author, std::string_view title, int year, Genre genre, double rating,
                   int read_count)
        : author(author), title(title), year(year), genre(genre), rating(rating), read_count(read_count) {};

    constexpr Book(std::string_view author, std::string_view title, int year, std::string_view genre, double rating,
                   int read_count)
        : author(author), title(title), year(year), genre(GenreFromString(genre)), rating(rating),
          read_count(read_count) {};

    auto operator<=>(const Book& other) const = default;
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext& fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Ваш код для std::formatter<Book> здесь

template <>
struct formatter<bookdb::Book, char> {
    bool short_format = false;

    template <typename FormatContext>
    auto format(const bookdb::Book book, FormatContext& fc) const {
        if (short_format) {
            return format_to(fc.out(), "[{}, {}]", book.author, book.year);
        } else {
            return std::format_to(fc.out(), "Author: {}, Title: {}, Year: {}, Genre: {}, Rating: {}, Reads: {}",
                                  book.author, book.title, book.year, book.genre, book.rating, book.read_count);
        }
    }

    constexpr auto parse(format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it == '#') {
            short_format = true;
            ++it;
        }
        return it;
    }
};

}  // namespace std
