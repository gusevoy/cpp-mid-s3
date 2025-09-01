#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T>& containter, Comparator comp = {}) {
    std::flat_map<std::string_view, size_t> histogram;
    std::for_each(containter.cbegin(), containter.cend(), [&histogram](const auto& book) {
        auto [it, inserted] = histogram.try_emplace(book.author, 0);
        it->second++;
    });
    return histogram;
}

template <BookIterator It, BookSentinel<It> Sentinel>
auto calculateGenreRatings(It begin, Sentinel end) {
    std::flat_map<Genre, std::pair<double, std::size_t>, std::less<void>> statistic;
    std::for_each(begin, end, [&statistic](const auto& book) {
        auto [it, inserted] = statistic.try_emplace(book.genre, 0.0, 0);
        it->second.first += book.rating;
        it->second.second++;
    });

    std::flat_map<Genre, double> histogram;

    std::for_each(statistic.cbegin(), statistic.cend(), [&histogram](const auto& stat) {
        const auto& [rating_sum, count] = stat.second;
        auto [it, inserted] = histogram.try_emplace(stat.first, 0.0);
        it->second = rating_sum / count;
    });

    return histogram;
}
}  // namespace bookdb

template <typename Compare, typename StringLike, typename Counter, typename KeyContainer, typename MappedContainer>
struct std::formatter<std::flat_map<StringLike, Counter, Compare, KeyContainer, MappedContainer>, char> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const std::flat_map<std::string_view, unsigned long>& histogram, std::format_context& ctx) const {
        auto out = ctx.out();

        out = std::format_to(out, "{{");
        bool first = true;

        for (const auto& [author, count] : histogram) {
            if (!first) {
                out = std::format_to(out, ", ");
            }
            out = std::format_to(out, "\"{}\": {}", author, count);
            first = false;
        }

        out = std::format_to(out, "}}");
        return out;
    }
};

template <typename Compare, typename KeyContainer, typename MappedContainer>
struct std::formatter<std::flat_map<bookdb::Genre, double, Compare, KeyContainer, MappedContainer>, char> {

    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const auto& genre_ratings, std::format_context& ctx) const {
        auto out = ctx.out();

        out = std::format_to(out, "{{\n");
        bool first = true;

        for (const auto& [genre, rating] : genre_ratings) {
            if (!first)
                out = std::format_to(out, ",\n");

            // Преобразуем Genre в строку
            std::string genre_str;
            for (const auto& [g, name] : bookdb::genreMappings) {
                if (g == genre) {
                    genre_str = name;
                    break;
                }
            }

            out = std::format_to(out, "  \"{}\": {:.2f}", genre_str, rating);
            first = false;
        }

        out = std::format_to(out, "\n}}");
        return out;
    }
};