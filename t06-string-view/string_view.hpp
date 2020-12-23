#pragma once
#ifndef __STRING_VIEW_HPP_
#define __STRING_VIEW_HPP_

#include <cstdint>
#include <string>

class StringView {
public:
    inline static constexpr std::size_t npos = std::size_t(-1);

    StringView(const char *s);

    StringView(const std::string &s);

    StringView(const char *ptr, std::size_t size);

    StringView(const StringView &sv) = default;

    const char *data() const;

    std::size_t size() const;

    StringView &operator=(const StringView &other);

    StringView &operator=(const char *other);

    StringView &operator=(const std::string &other);

    StringView substr(std::size_t start, std::size_t end = npos) const;

    char operator[](std::size_t index) const;

    std::size_t find(char c) const;

    std::size_t find(const StringView &s) const;

    bool startsWith(const StringView &other) const;

    bool startsWith(char c) const;

    bool endsWith(const StringView &other) const;

    bool endsWith(char c) const;

    std::size_t findFirstOf(char x, std::size_t index = 0) const;

    std::size_t findFirstOf(const StringView &s, std::size_t index = 0) const;

    std::size_t findFirstNotOf(char x, std::size_t index = 0) const;

    std::size_t findFirstNotOf(const StringView &s, std::size_t index = 0) const;

    void removePrefix(size_t pref);

private:
    friend void swap(StringView &s1, StringView &s2);

    std::size_t findFirstWrap(const StringView &s, std::size_t = 0, bool = true) const;

    const char *begin_;
    std::size_t size_;
};

#endif
