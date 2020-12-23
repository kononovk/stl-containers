#pragma once

#include <cstdint>
#include <stdexcept>
#include <algorithm>

namespace cls_08 {
    template<class T>
    class list {
    public:
        list *next;
        T value;
    };

    template<class T>
    class queue {
    public:
        queue() = default;
        queue(const queue& other) {
            size_ = other.size_;
            if (other.size() == 0) return;
            q_begin_ = new list<T>{nullptr, other.q_begin_->value};
            list<T>* tmp = other.q_begin_;
            auto q_begin_tmp = q_begin_;
            tmp = tmp->next;
            while(tmp) {
                auto *new_node = new list<T>{nullptr, tmp->value};
                q_begin_tmp->next = new_node;
                q_begin_tmp = q_begin_tmp->next;
                tmp = tmp->next;
            }
            q_end_ = q_begin_tmp;
        }
        queue& operator=(const queue& other) {
            queue copy(other);
            swap(copy);
            return *this;
        }

        void swap(queue<T>& other) {
            std::swap(q_begin_, other.q_begin_);
            std::swap(q_end_, other.q_end_);
            std::swap(size_, other.size_);
        }

        ~queue() {
            if (q_begin_ == nullptr)
                return;
            while (q_begin_->next != nullptr) {
                auto *tmp = q_begin_->next;
                delete q_begin_;
                q_begin_ = tmp;
            }
            delete q_begin_;
        }

        void push(const T &value) {
            auto *tmp = new list<T>{nullptr, value};
            if (size_ != 0) {
                q_end_->next = tmp;
            }
            q_end_ = tmp;
            if (size_ == 0) {
                q_begin_ = q_end_;
            }
            size_++;
        }

        void pop() {
            if (size_ == 0) {
                throw std::runtime_error("queue is empty");
            }
            list<T> *tmp = q_begin_->next;
            delete q_begin_;
            q_begin_ = tmp;
            size_--;
        }

        T &front() {
            return q_begin_->value;
        }

        const T &front() const {
            return q_begin_->value;
        }

        std::size_t size() const {
            return size_;
        }

    private:
        std::size_t size_ = 0;
        list<T> *q_end_ = nullptr;
        list<T> *q_begin_ = nullptr;
    };
}