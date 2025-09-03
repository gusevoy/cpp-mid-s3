#pragma once

#include <concepts>
#include <print>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using value_type = typename BookContainer::value_type;
    using allocator_type = typename BookContainer::allocator_type;
    using size_type = BookContainer::size_type;
    using difference_type = BookContainer::difference_type;
    using reference = BookContainer::reference;
    using const_reference = BookContainer::const_reference;
    using pointer = BookContainer::pointer;
    using const_pointer = BookContainer::const_pointer;
    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;
    using reverse_iterator = BookContainer::reverse_iterator;
    using const_reverse_iterator = BookContainer::const_reverse_iterator;

    // Ваш код здесь

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;

    constexpr BookDatabase(std::initializer_list<Book> books) {
        if constexpr (requires { books_.reserve(size_type{}); }) {
            books_.reserve(books.size());
        }
        for (const auto& book : books) {
            push_back(book);
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    constexpr bool empty() const noexcept { return books_.empty(); }
    constexpr size_type size() const noexcept { return books_.size(); }

    constexpr iterator begin() noexcept { return books_.begin(); }
    constexpr iterator end() noexcept { return books_.end(); }
    constexpr const_iterator begin() const noexcept { return books_.begin(); }
    constexpr const_iterator end() const noexcept { return books_.end(); }
    constexpr const_iterator cbegin() const noexcept { return books_.cbegin(); }
    constexpr const_iterator cend() const noexcept { return books_.cend(); }
    constexpr reverse_iterator rbegin() noexcept { return books_.rbegin(); }
    constexpr reverse_iterator rend() noexcept { return books_.rend(); }
    constexpr const_reverse_iterator crbegin() const noexcept { return books_.crbegin(); }
    constexpr const_reverse_iterator crend() const noexcept { return books_.crend(); }

    // Ваш код здесь
    template <typename T>
        requires std::convertible_to<T, value_type>
    constexpr void push_back(T&& book) {
        value_type book_copy = std::forward<T>(book);
        book_copy.author = ensure_author(book_copy.author);
        books_.push_back(std::move(book_copy));
    }

    template <typename... Args>
    constexpr reference emplace_back(Args&&... args) {
        value_type book{std::forward<Args>(args)...};
        book.author = ensure_author(book.author);
        return books_.emplace_back(std::move(book));
    }

    const BookContainer& get_books() const { return books_; }
    const AuthorContainer& get_authors() const { return authors_; }

private:
    std::string_view ensure_author(std::string_view author) {
        if (author.empty()) {
            return {};
        }
        return *(authors_.emplace(author).first);
    }

    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>>& db, FormatContext& fc) const {
        // Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        // (size/begin/...)

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto& book : db.get_books()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto& author : db.get_authors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
