#pragma once

#include <concepts>
#include <iterator>
#include <utility>

#include "book.hpp"

namespace bookdb {

template <typename Container>
concept BookContainerLike = requires(Container cont, const Container ccont) {
    typename Container::value_type;
    requires std::convertible_to<typename Container::value_type, Book>;
    typename Container::allocator_type;
    typename Container::size_type;
    typename Container::difference_type;
    typename Container::reference;
    typename Container::const_reference;
    typename Container::pointer;
    typename Container::const_pointer;
    typename Container::iterator;
    typename Container::const_iterator;
    typename Container::reverse_iterator;
    typename Container::const_reverse_iterator;
    { cont.begin() } -> std::same_as<typename Container::iterator>;
    { cont.end() } -> std::sentinel_for<typename Container::iterator>;
    { cont.cbegin() } -> std::same_as<typename Container::const_iterator>;
    { cont.cend() } -> std::sentinel_for<typename Container::const_iterator>;
    { cont.rbegin() } -> std::same_as<typename Container::reverse_iterator>;
    { cont.rend() } -> std::sentinel_for<typename Container::reverse_iterator>;
    { cont.crbegin() } -> std::same_as<typename Container::const_reverse_iterator>;
    { cont.crend() } -> std::sentinel_for<typename Container::const_reverse_iterator>;
    { cont.empty() } -> std::convertible_to<bool>;
    { cont.size() } -> std::convertible_to<typename Container::size_type>;
    { cont.clear() };
    { cont.push_back(std::declval<typename Container::value_type>()) };
    { cont.emplace_back() } -> std::convertible_to<typename Container::value_type>;
};

template <typename T>
concept BookIterator = std::forward_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I> && BookIterator<I>;

template <typename P>
concept BookPredicate = std::predicate<P, const Book&>;

template <typename C>
concept BookComparator =
    std::relation<C, const Book&, const Book&> && requires(C comparator, const Book& lhs, const Book& rhs) {
        { comparator(lhs, rhs) } -> std::convertible_to<bool>;
    };

}  // namespace bookdb