#pragma once

#include <type_traits>
#include <stdexcept>
#include <utility>

namespace utils {
    class bad_whatever_cast : public std::bad_cast {
    public:
        const char *what() const noexcept override {
            return "bad_whatever_cast: failed conversion using whatever_cast";
        }
    };

    class whatever {
    public:
        whatever() : holder_(nullptr) {}

        // https://clck.ru/S8E2H to see cpp insights
        template<class T,
                class IsNotWhatever = std::enable_if_t<!std::is_same_v<whatever, std::decay_t<T>>>>
        whatever(T &&value) : holder_(
                new ObjectHolder<std::remove_cv_t<std::decay_t<const T>>>(std::forward<T>(value))) {
        }

        whatever(const whatever &other) : holder_(other.holder_ ? other.holder_->clone() : nullptr) {
        }

        whatever(whatever &&other) = default;

        template<class T>
        whatever &operator=(T &&value) {
            whatever(std::forward<T>(value)).swap(*this);
            return *this;
        }

        whatever &operator=(const whatever &other) {
            whatever(other).swap(*this);
            return *this;
        }

        whatever &operator=(whatever &&other) noexcept = default;

        ~whatever() {
            delete holder_;
        }

        bool empty() const noexcept {
            return !holder_;
        }

        void clear() noexcept {
            delete holder_;
            holder_ = nullptr;
        }

        const std::type_info &type_info() const {
            return this->empty() ? typeid(void) : holder_->type_info();
        }

        void swap(whatever &other) noexcept {
            BaseHolder *tmp = holder_;
            holder_ = other.holder_;
            other.holder_ = tmp;
        }

    private:
        struct BaseHolder {
            virtual ~BaseHolder() noexcept = default;

            virtual BaseHolder *clone() const = 0;

            virtual const std::type_info &type_info() const noexcept = 0;
        };

        template<class T>
        struct ObjectHolder final : BaseHolder {
            explicit ObjectHolder(const T &t) : t_(t) {
            }

            ObjectHolder(T &&t) : t_(std::move(t)) {
            }

            ~ObjectHolder() noexcept override = default;

            ObjectHolder &operator=(const ObjectHolder &) = delete;

            ObjectHolder &operator=(ObjectHolder &&) = delete;

            const std::type_info &type_info() const noexcept override {
                return typeid(t_);
            }

            ObjectHolder<T> *clone() const override {
                return new ObjectHolder(t_);
            }

            T t_;
        };

        template<class T>
        friend const T *whatever_cast(const whatever *);

        BaseHolder *holder_ = nullptr;
    };


    inline void swap(whatever &lhs, whatever &rhs) noexcept {
        lhs.swap(rhs);
    }

    template<class T>
    const T *whatever_cast(const whatever *w) {
        if (w == nullptr || typeid(std::decay_t<T>) != w->type_info()) {
            return nullptr;
        }
        return std::addressof(static_cast<whatever::ObjectHolder<std::decay_t<T>> *>(w->holder_)->t_);
    }

    template<class T>
    T *whatever_cast(whatever *w) {
        return const_cast<T *>(whatever_cast<T>(const_cast<const whatever *>(w)));
    }

    template<class T>
    T whatever_cast(const whatever &w) {
        if (auto *ptr = whatever_cast<std::decay_t<T>>(&w); ptr) {
            return *ptr;
        }
        throw bad_whatever_cast{};
    }

    template<class T>
    T whatever_cast(whatever &w) {
        using nonref = std::remove_reference_t<T>;
        return const_cast<nonref &>(whatever_cast<const nonref &>(static_cast<const whatever &>(w)));
    }
} // namespace utils
