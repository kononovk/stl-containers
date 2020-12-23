#pragma once
#ifndef __LAZY_STRING_HPP_
#define __LAZY_STRING_HPP_

#include <cstdint>
#include "shared_buffer.hpp"

class CharWrapper;

class LazyString {
public:
    friend class CharWrapper;

    LazyString();

    LazyString(char const *str);

    LazyString(const char *str, std::size_t size);

    LazyString(SharedBuffer &other);

    LazyString(const LazyString &other) = default;

    LazyString &operator=(const LazyString &other);

    LazyString &operator=(char const *str);

    std::size_t getSize() const;

    const char *cStr() const;

    std::size_t useCount() const;

    char at(std::size_t index) const;

    LazyString &operator+=(const LazyString &other);

    char operator[](std::size_t index) const;

    CharWrapper operator[](std::size_t index);

    friend LazyString operator+(const LazyString &, const LazyString &);

private:
    friend void swap(LazyString &lhs, LazyString &rhs);

    char *non_const_cStr() const;

    SharedBuffer data_;
};

LazyString operator+(const LazyString &, const LazyString &);

bool operator==(const LazyString &, const LazyString &rhs);

bool operator!=(const LazyString &, const LazyString &);

class CharWrapper {
public:
    CharWrapper(char a);

    CharWrapper(char a, std::size_t index, LazyString *parent);

    CharWrapper &operator=(const CharWrapper &other);

    CharWrapper &operator++();

    char operator++(int);

    operator char() {
        return value;
    }

private:
    char value = 0;
    std::size_t id_;
    LazyString *parent_string = nullptr;
};

#endif