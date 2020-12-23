#include "queue.hpp"

#include <cassert>
#include <string>
#include <list>
#include <stdexcept>

static void test_queue() {
    {
        cls_08::queue<int> q;
    }

    {
        cls_08::queue<std::string> initial;
        cls_08::queue<std::string> const other(initial);

        initial.push("1");
        assert(initial.size() == 1 && other.size() == 0);

        initial = static_cast<cls_08::queue<std::string> const>(initial);
        assert(initial.size() == 1 && other.size() == 0);

        initial = other;
        assert(initial.size() == 0 && other.size() == 0);
    }

    {
        cls_08::queue<int> queue;
        cls_08::queue<int> const& ref = queue;

        auto check_queue_state = [&](size_t size, int front) {
            assert(queue.size() == size && queue.front() == front); \
            assert(ref.size() == size && ref.front() == front);
        };

        queue.push(0);
        check_queue_state(1, 0);

        queue.push(1);
        check_queue_state(2, 0);

        queue.push(2);
        check_queue_state(3, 0);

        queue.pop();
        check_queue_state(2, 1);

#undef CHECK_QUEUE_STATE
    }

    {
        cls_08::queue<int> queue;
        std::list<int> original;

        auto push_to_both = [&](int i) {
            queue.push(i);
            original.push_back(i);
        };

        auto pop_from_both = [&]() {
            if (queue.size() != 0) {
                queue.pop();
            }

            if (!original.empty()) {
                original.pop_front();
            }
        };

        auto assert_consistent = [&]() {
            assert(queue.size() == original.size());
            if (queue.size() != 0) {
                assert(queue.front() == original.front());
            }
        };

        for (int i = 0; i < 1024; ++i) {
            if (i % 3 != 0) {
                push_to_both(i);
            } else {
                pop_from_both();
            }

            assert_consistent();
        }
    }

    {
        static size_t counter{0};

        struct throws_on_copy {
            throws_on_copy() = default;
            throws_on_copy(throws_on_copy const& other) {
                if (++counter % 3 == 0) {
                    throw std::runtime_error("Exception from copy constructor");
                }
            }
        };

        cls_08::queue<throws_on_copy> queue;

        for (int i = 0; i < 100; ++i) {
            throws_on_copy element;

            size_t previous_size = queue.size();
            try {
                queue.push(element);
            } catch (std::runtime_error const&) {
                assert(previous_size == queue.size());
            }
        }
    }
}

static void test_copy_construct() {
    for (size_t i = 0; i < 5; i++) {
        cls_08::queue<int> q;
        for (size_t j = 0; j < i; j++) {
            q.push(j);
        }

        assert(q.size() == i);
        {
            cls_08::queue<int> other_q = q;
            for (size_t j = 0; j < i; j++) {
                other_q.pop();
            }
            assert(other_q.size() == 0);
        }

        assert(q.size() == i);
        for (size_t j = 0; j < i; j++) {
            q.pop();
        }
        assert(q.size() == 0);
    }
}

static void test_copy_assign() {
    cls_08::queue<int> q;
    q.push(0);
    q.push(1);
    q.push(2);

    cls_08::queue<int> other_q;
    other_q.push(-1);
    other_q.push(-2);

    other_q = q;
    assert(other_q.size() == 3);
    assert(other_q.front() == 0);

    other_q.pop();
    assert(other_q.size() == 2);
    assert(q.size() == 3);

    q.pop();
    q.pop();
    q.pop();
    assert(q.size() == 0);
}

int main() {
    test_copy_assign();
    test_copy_construct();
    test_queue();
}