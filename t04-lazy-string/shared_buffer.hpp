#pragma once
#ifndef __SHARED_BUFFER_HPP_
#define __SHARED_BUFFER_HPP_

#include <cstdint>

class SharedBuffer {
public:
    explicit SharedBuffer();

    explicit SharedBuffer(std::size_t size);

    SharedBuffer(const SharedBuffer &buffer);

    ~SharedBuffer();

    SharedBuffer &operator=(const SharedBuffer &other);

    std::size_t getSize() const;

    std::size_t useCount() const;

    char *getData() const;

    friend void swap(SharedBuffer &a, SharedBuffer &b);
private:

    std::size_t size_ = 0;
    char *arr_ = nullptr;
    std::size_t *ref_count = nullptr;
};

#endif