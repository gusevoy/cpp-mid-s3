#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

constexpr auto YearBetween(int from, int to) {
    return [from, to](const Book& book) { return from <= book.year && book.year <= to; };
}

constexpr auto RatingAbove(double raiting) {
    return [raiting](const Book& book) { return book.rating >= raiting; };
}

constexpr auto GenreIs(Genre genre) {
    return [genre](const Book& book) { return book.genre == genre; };
}

template <typename... Predicate>
constexpr auto any_of(Predicate&&... predicates) {
    return
        [... predicates = std::forward<Predicate>(predicates)](const Book& book) { return (predicates(book) || ...); };
}

template <typename... Predicate>
constexpr auto all_of(Predicate&&... predicates) {
    return
        [... predicates = std::forward<Predicate>(predicates)](const Book& book) { return (predicates(book) && ...); };
}

template <BookIterator It, BookSentinel<It> Sentinel, BookPredicate Predicate>
constexpr std::vector<std::reference_wrapper<const Book>> filterBooks(const It begin, const Sentinel end,
                                                                      Predicate predicate) {
    std::vector<std::reference_wrapper<const Book>> result;
    std::copy_if(begin, end, std::back_inserter(result), [&predicate](const Book& book) { return predicate(book); });
    return result;
}

}  // namespace bookdb