#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <mutex>
#include <numeric>
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
        // похоже в GCC15 какой-то баг, который не дает сделать просто ++histogram[book.author]
        // поэтому везде ниже вот такие выкрутасы с try_emplace
        ++histogram.try_emplace(book.author, 0).first->second;
    });
    return histogram;
}

template <BookIterator It, BookSentinel<It> Sentinel>
auto calculateGenreRatings(It begin, Sentinel end) {
    std::flat_map<Genre, std::pair<double, std::size_t>, std::less<void>> statistic;
    std::for_each(begin, end, [&statistic](const auto& book) {
        auto [it, _] = statistic.try_emplace(book.genre, 0.0, 0);
        it->second.first += book.rating;
        ++it->second.second;
    });

    std::flat_map<Genre, double> histogram;
    std::for_each(statistic.cbegin(), statistic.cend(), [&histogram](const auto& stat) {
        const auto& [rating_sum, count] = stat.second;
        auto [it, inserted] = histogram.try_emplace(stat.first, 0.0);
        it->second = rating_sum / count;
    });

    return histogram;
}

template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T>& containter) {
    if (containter.empty()) {
        return 0.0;
    }
    return std::accumulate(containter.cbegin(), containter.cend(), 0.0,
                           [](auto sum, const auto& book) { return sum + book.rating; }) /
           containter.size();
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T>& containter, size_t count) {
    std::vector<std::reference_wrapper<const Book>> res;
    res.reserve(count);
    std::sample(containter.cbegin(), containter.cend(), std::back_inserter(res), count,
                std::mt19937{std::random_device{}()});
    return res;
}

template <BookContainerLike T, BookComparator Comparator>
auto getTopNBy(BookDatabase<T>& containter, size_t n, Comparator comparator) {
    n = std::min(n, containter.size());
    auto mid = std::next(containter.begin(), n);
    std::partial_sort(containter.begin(), mid, containter.end(), comparator);
    return std::vector<std::reference_wrapper<const Book>>{containter.begin(), mid};
}

}  // namespace bookdb

template <typename TextLike, typename Counter, typename Compare, typename KeyContainer, typename MappedContainer>
struct std::formatter<std::flat_map<TextLike, Counter, Compare, KeyContainer, MappedContainer>, char> {

    using FlatMap = std::flat_map<TextLike, Counter, Compare, KeyContainer, MappedContainer>;

    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    auto format(const FlatMap& histogram, std::format_context& ctx) const {
        auto out = ctx.out();

        out = std::format_to(out, "{{");
        bool first = true;

        for (const auto& [text, count] : histogram) {
            if (!first) {
                out = std::format_to(out, ", ");
            }
            out = std::format_to(out, "\"{}\": {}", text, count);
            first = false;
        }

        out = std::format_to(out, "}}");
        return out;
    }
};
