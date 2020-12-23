#pragma once

namespace control_03 {

template<class T>
class maybe {
public:
    maybe() = default;

    maybe(const T &val) : has_value_(true) {
        new(m_storage_.buf_) T(val);
    }

    maybe(T &&other) : has_value_(true) {
        new(get_raw_memory()) T(std::move(other));
    }

    maybe(const maybe &source) : has_value_(source.has_value_) {
        if (source.has_value()) {
            new(m_storage_.buf_) T(source.value());
        }
    }

    maybe(maybe &&source) noexcept: has_value_(source.has_value_) {
        if (source.has_value()) {
            new(m_storage_.buf_) T(std::move(source.value()));
        }
    }

    maybe &operator=(maybe &&other) noexcept {
        if (this == &other) { return *this; }

        if (other.has_value()) {
            if (has_value()) {
                value() = std::move(other.value());
            } else {
                new(m_storage_.buf_) T(std::move(other.value()));
            }
        } else {
            destroy();
        }

        has_value_ = other.has_value_;
        return *this;
    }

    maybe &operator=(const maybe &other) {
        if (has_value_ && other.has_value_)
            *(*this) = *other;
        else if (has_value_ && !other.has_value_)
            reset();
        else if (!has_value_ && other.has_value_) {
            new(reinterpret_cast<T *>(m_storage_.buf_)) T(*other);
            has_value_ = true;
        }
        return *this;
    }

    const T &value() const {
        return *(this->get_raw_memory());
    }

    T &value() {
        return *(this->get_raw_memory());
    }

    bool has_value() const {
        return has_value_;
    }

    ~maybe() {
        destroy();
    }

    operator bool() {
        return has_value();
    }

    void destroy() {
        if (has_value()) {
            value().~T();
            has_value_ = false;
        }
    }

    void reset(T &&x) {
        *this = std::move(x);
        has_value_ = true;
    }

    void reset(const T &x) {
        if (has_value()) {
            *this = x;
        }
        has_value_ = true;
    }

    void reset() {
        destroy();
    }

    template<class... Args>
    void emplace(Args &&... args) {
        this->reset();
        new(reinterpret_cast<T *>(m_storage_.buf_)) T(std::forward<Args>(args)...);
        has_value_ = true;
    }

    T &operator*() {
        return *get_raw_memory();
    }

    const T &operator*() const {
        return *get_raw_memory();
    }

private:
    bool has_value_ = false;
    union {
        alignas(T) unsigned char buf_[sizeof(T)] {};
        char failed_byte;
    } m_storage_;

    T *get_raw_memory() {
        return reinterpret_cast<T *>(m_storage_.buf_);
    }

    const T *get_raw_memory() const {
        return reinterpret_cast<const T *>(m_storage_.buf_);
    }
};

template<class T>
class maybe<T *> {
public:
    maybe() = default;

    maybe(T *other) { data_ = other; }

    maybe &operator=(T *other) {
        data_ = other;
        return *this;
    }

    maybe(const maybe &other) : data_(other.data_) {
    }

    maybe &operator=(maybe other) {
        data_ = other.data_;
        return *this;
    }

    maybe(maybe &&other) noexcept: data_(other.data_) {
        other.data_ = nullptr;
    }

    maybe &operator=(maybe &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        data_ = other.data_;
        other.data_ = nullptr;
        return *this;
    }

    void reset(T *other = nullptr) {
        data_ = other;
    }

    T *value() const {
        return data_;
    }

    const T *value() {
        return data_;
    }

    operator bool() {
        return data_;
    }

    bool has_value() {
        return data_;
    }

private:
    T *data_ = nullptr;
};
}

