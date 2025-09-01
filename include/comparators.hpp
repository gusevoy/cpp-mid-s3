#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author < rhs.author; }
};

struct GreaterByAuthor {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.author > rhs.author; }
};

struct LessByTitle {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title < rhs.title; }
};

struct GreaterByTitle {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.title > rhs.title; }
};

struct LessByRating {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating < rhs.rating; }
};

struct GreaterByRating {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating > rhs.rating; }
};

struct LessByYear {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.year < rhs.year; }
};

struct GreaterByYear {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.year > rhs.year; }
};

struct LessByReadCount {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count < rhs.read_count; }
};

struct GreaterByReadCount {
    constexpr bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count > rhs.read_count; }
};

}  // namespace bookdb::comp