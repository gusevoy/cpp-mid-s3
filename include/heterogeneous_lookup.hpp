#pragma once

#include <string>
#include <string_view>

#include "book.hpp"

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    auto operator()(const Book& lhs, const Book& rhs) const noexcept { return lhs.title < rhs.title; }
    auto operator()(std::string_view lhs, const Book& rhs) const noexcept { return lhs < rhs.title; }
    auto operator()(const Book& lhs, std::string_view rhs) const noexcept { return lhs.title < rhs; }
    auto operator()(std::string_view lhs, std::string_view rhs) const noexcept { return lhs < rhs; }
};

struct TransparentStringEqual {
    using is_transparent = void;

    auto operator()(const Book& lhs, const Book& rhs) const noexcept { return lhs.title == rhs.title; }
    auto operator()(const Book& lhs, std::string_view rhs) const noexcept { return lhs.title == rhs; }
    auto operator()(std::string_view lhs, const Book& rhs) const noexcept { return lhs == rhs.title; }
    auto operator()(std::string_view lhs, std::string_view rhs) const noexcept { return lhs == rhs; }
};

struct TransparentStringHash {
    using is_transparent = void;

    auto operator()(const Book& book) const noexcept { return std::hash<std::string>{}(book.title); }
    auto operator()(std::string_view title) const noexcept { return std::hash<std::string_view>{}(title); }
};

struct TransparentAuthorLess {
    using is_transparent = void;

    auto operator()(const Book& lhs, const Book& rhs) const noexcept { return lhs.author < rhs.author; }
    auto operator()(std::string_view lhs, const Book& rhs) const noexcept { return lhs < rhs.author; }
    auto operator()(const Book& lhs, std::string_view rhs) const noexcept { return lhs.author < rhs; }
    auto operator()(std::string_view lhs, std::string_view rhs) const noexcept { return lhs < rhs; }
};

}  // namespace bookdb
