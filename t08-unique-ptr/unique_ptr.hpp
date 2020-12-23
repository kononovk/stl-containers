#include <utility>

#pragma once

namespace cls09 {
    template<class ValueType>
    class unique_ptr {
    public:
        constexpr unique_ptr() noexcept = default;

        unique_ptr(ValueType *ptr) : pointer_(ptr) {
        }

        unique_ptr &operator=(ValueType *ptr) {
            delete pointer_;
            pointer_ = ptr;
        }

        unique_ptr(unique_ptr &&other) noexcept {
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
        }

        unique_ptr &operator=(unique_ptr &&other) noexcept {
            delete pointer_;
            pointer_ = other.pointer_;
            other.pointer_ = nullptr;
            return *this;
        }

        ValueType *release() {
            auto tmp = pointer_;
            pointer_ = nullptr;
            return tmp;
        }

        unique_ptr(const unique_ptr &data) = delete;

        unique_ptr &operator=(const unique_ptr &) = delete;


        ~unique_ptr() {
            delete pointer_;
        }

        void reset(ValueType *new_object = nullptr) {
            delete pointer_;
            pointer_ = new_object;
        }

        ValueType *get() {
            return pointer_;
        }

        operator bool() {
            return pointer_;
        }

        ValueType *operator->() {
            return pointer_;
        }

        ValueType operator*() {
            return *pointer_;
        }

    private:
        ValueType *pointer_ = nullptr;
    };

    template<class T, class... Args>
    unique_ptr<T> make_unique(Args &&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
