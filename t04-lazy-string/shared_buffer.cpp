#include "shared_buffer.hpp"
#include <cstdint>
#include <algorithm>

using std::size_t;

SharedBuffer::SharedBuffer() :
        size_(0),
        arr_(nullptr),
        ref_count(nullptr) {
}

SharedBuffer::SharedBuffer(size_t size) :
        size_(size) {
    arr_ = new char[size_]{};
    ref_count = new size_t{1};
}

SharedBuffer::SharedBuffer(const SharedBuffer &buffer) :
        size_(buffer.size_),
        arr_(buffer.arr_),
        ref_count(buffer.ref_count) {
    (*ref_count)++;
}

SharedBuffer::~SharedBuffer() {
    if (ref_count != nullptr && (*ref_count)-- <= 1) {
        delete[] arr_;
        delete ref_count;
        arr_ = nullptr;
        ref_count = nullptr;
    }
}

void swap(SharedBuffer &a, SharedBuffer &b) {
    std::swap(a.size_, b.size_);
    std::swap(a.arr_, b.arr_);
    std::swap(a.ref_count, b.ref_count);
}

SharedBuffer &SharedBuffer::operator=(const SharedBuffer &other) {
    if (this == &other) return *this;
    SharedBuffer tmp(other);
    swap(tmp, *this);
    return *this;
}

size_t SharedBuffer::getSize() const {
    return size_;
}

size_t SharedBuffer::useCount() const {
    return *ref_count;
}

char *SharedBuffer::getData() const {
    return arr_;
}

