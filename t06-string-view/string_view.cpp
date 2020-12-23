#include <cstring>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "string_view.hpp"

StringView::StringView(const std::string &s) : begin_(s.c_str()), size_(s.size()) {
}

StringView::StringView(const char *ptr, std::size_t size) : begin_(ptr), size_(size) {
}

StringView::StringView(const char *s) : begin_(s), size_(0) {
    if (s != nullptr)
        while (*(s + size_) != '\0') size_++;
}

void swap(StringView &s1, StringView &s2) {
    std::swap(s1.begin_, s2.begin_);
    std::swap(s1.size_, s2.size_);
}

StringView &StringView::operator=(const StringView &other) {
    if (this == &other)
        return *this;
    StringView copy(other);
    swap(*this, copy);
    return *this;
}

StringView &StringView::operator=(const char *other) {
    StringView copy(other);
    swap(*this, copy);
    return *this;
}

StringView &StringView::operator=(const std::string &other) {
    StringView copy(other);
    swap(*this, copy);
    return *this;
}

const char *StringView::data() const {
    return begin_;
}

std::size_t StringView::size() const {
    return size_;
}

StringView StringView::substr(std::size_t start, std::size_t end) const {
    end = std::min(end, size_) - start;
   // end = (end == npos ? size_ : end) - start;
    if (end > size_) end = size_;
    return StringView(begin_ + start, end);
}

char StringView::operator[](std::size_t index) const {
    return begin_[index];
}

void StringView::removePrefix(std::size_t pref) {
    if (pref >= size_) pref = size_ - 1;
    begin_ += pref;
    size_ -= pref;
}

std::size_t StringView::find(const StringView &s) const {
    std::size_t n = s.size();
    if (n > size_)
        return npos;
    for (size_t i = 0; i <= size_ - n; ++i) {
        bool is_substr = true;
        for (std::size_t j = 0; j < n; ++j) {
            if (begin_[i + j] != s[j]) {
                is_substr = false;
                break;
            }
        }
        if (is_substr) {
            return i;
        }
    }
    return npos;
}

std::size_t StringView::find(char c) const {
    return find(StringView(&c, 1));
}

bool StringView::startsWith(const StringView &other) const {
    std::size_t i = 0;
    for (; i < size_ && i < other.size(); ++i) {
        if (begin_[i] != other.begin_[i]) {
            return false;
        }
    }
    return i <= size_;
}

bool StringView::startsWith(char c) const {
    return begin_[0] == c;
}

bool StringView::endsWith(const StringView &other) const {
    if (other.size() > size())
        return false;
    for (std::size_t i = 0; i < other.size(); ++i) {
        if (begin_[size_ - other.size() + i] != other.data()[i])
            return false;
    }
    return true;
}

bool StringView::endsWith(char c) const {
    return begin_[size_ - 1] == c;
}

std::size_t StringView::findFirstWrap(const StringView &s,
                                      std::size_t index,
                                      bool flag) const {
    if (index > size_)
        return npos;
    std::unordered_set<char> flags{s.begin_, s.begin_ + s.size_};
    for (std::size_t i = index; i < size_; ++i) {
        if (flag == flags.contains(begin_[i]))
            return i;
    }
    return npos;
}

std::size_t StringView::findFirstOf(const StringView &s, std::size_t index) const {
    return findFirstWrap(s, index);
}

std::size_t StringView::findFirstNotOf(const StringView &s, std::size_t index) const {
    return findFirstWrap(s, index, false);
}


std::size_t StringView::findFirstOf(char x, std::size_t index) const {
    const char *tmp = &x;
    return this->findFirstOf(StringView(tmp, 1), index);
}

std::size_t StringView::findFirstNotOf(char x, std::size_t index) const {
    const char *tmp = &x;
    return this->findFirstNotOf(StringView(tmp, 1), index);
}
