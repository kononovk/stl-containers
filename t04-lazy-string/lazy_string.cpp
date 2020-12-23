#include "lazy_string.hpp"
#include "shared_buffer.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>

LazyString::LazyString() {
    data_ = SharedBuffer(1);
    data_.getData()[0] = '\0';
}

LazyString::LazyString(char const *str) {
    std::size_t size_ = std::strlen(str);
    data_ = SharedBuffer(size_ + 1);
    std::strcpy(data_.getData(), str);
    data_.getData()[size_] = '\0';
}

LazyString::LazyString(SharedBuffer &other) {
    data_ = other;
}

LazyString::LazyString(const char *str, std::size_t size) {
    std::size_t size_ = size;
    data_ = SharedBuffer(size_ + 1);
    std::copy(str, str + size_, data_.getData());
    data_.getData()[size_] = '\0';
}

void swap(LazyString &lhs, LazyString &rhs) {
    swap(lhs.data_, rhs.data_);
}

LazyString &LazyString::operator=(const LazyString &other) {
    if (this == &other) return *this;
    data_ = other.data_;
    return *this;
}

LazyString &LazyString::operator=(char const *str) {
    LazyString tmp(str);
    swap(*this, tmp);
    return *this;
}

char *LazyString::non_const_cStr() const {
    return data_.getData();
}

std::size_t LazyString::getSize() const {
    return data_.getSize() - 1;
}

const char *LazyString::cStr() const {
    return data_.getData();
}

std::size_t LazyString::useCount() const {
    return data_.useCount();
}

char LazyString::at(std::size_t index) const {
    if (index > this->getSize())
        throw std::invalid_argument("index > size");
    return data_.getData()[index];
}

LazyString &LazyString::operator+=(const LazyString &other) {
    std::size_t size = this->getSize() + other.getSize();
    SharedBuffer tmp(size + 1);
    std::copy(data_.getData(), data_.getData() + this->getSize(), tmp.getData());
    std::copy(other.data_.getData(),
              other.data_.getData() + other.getSize(),
              tmp.getData() + this->getSize());
    tmp.getData()[size] = '\0';
    swap(data_, tmp);
    return *this;
}

LazyString operator+(const LazyString &lhs, const LazyString &rhs) {
    LazyString res;
    std::size_t size = lhs.getSize() + rhs.getSize();
    SharedBuffer tmp(size + 1);
    std::copy(lhs.data_.getData(), lhs.data_.getData() + lhs.getSize(), tmp.getData());
    std::copy(rhs.data_.getData(), rhs.data_.getData() + rhs.getSize(), tmp.getData() + lhs.getSize());
    tmp.getData()[size] = '\0';
    return LazyString(tmp);
}

bool operator==(const LazyString &lhs, const LazyString &rhs) {
    if (&lhs == &rhs && lhs.getSize() == rhs.getSize())
        return true;
    char const *ptr1 = lhs.cStr();
    char const *ptr2 = rhs.cStr();
    while (ptr1 < lhs.cStr() + lhs.getSize() &&
           ptr2 < rhs.cStr() + rhs.getSize()) {
        if (*(ptr1++) != *(ptr2++)) return false;
    }
    return lhs.getSize() == rhs.getSize();
}

bool operator!=(const LazyString &lhs, const LazyString &rhs) {
    return !(lhs == rhs);
}

char LazyString::operator[](std::size_t index) const {
    return data_.getData()[index];
}

CharWrapper LazyString::operator[](std::size_t index) {
    return CharWrapper(data_.getData()[index], index, this);
}

// Char Wrapper methods
CharWrapper::CharWrapper(char a) : value(a) {}

CharWrapper::CharWrapper(char a, std::size_t index, LazyString *parent) :
        value(a),
        id_(index),
        parent_string(parent) {}


CharWrapper &CharWrapper::operator=(const CharWrapper &other) {
    if (this == &other)
        return *this;
    if (parent_string != nullptr && parent_string->useCount() > 1) {
        LazyString copy(parent_string->cStr(), parent_string->getSize());
        *parent_string = copy;
    }
    parent_string->non_const_cStr()[id_] = other.value;
    return *this;
}

CharWrapper &CharWrapper::operator++() {
    if (parent_string != nullptr && parent_string->useCount() > 1) {
        LazyString copy(parent_string->cStr(), parent_string->getSize());
        *parent_string = copy;
    }
    parent_string->non_const_cStr()[id_]++;
    return *this;
}

char CharWrapper::operator++(int) {
    char ans{this->value};
    ++(*this);
    return ans;
}

