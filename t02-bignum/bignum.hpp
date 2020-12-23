#pragma once
#ifndef __BIGNUM_HPP_
#define __BIGNUM_HPP_

#include <regex>     // std::regex
#include <iostream>  // std::ostream, std::istream
#include <cstdint>   // std::uint32_t, std::uint64_t
#include <vector>    // std::vector
#include <string>    // std::string
#include <cmath>     // pow
#include <algorithm> // std::max, std::swap

namespace mp {
    class bignum {
    public:
        bignum(std::uint32_t pref = 0);

        explicit bignum(const std::string &str);

        bignum(const bignum &other) = default;

        bignum &operator=(const std::string &s);

        bignum &operator=(const bignum &other);

        explicit operator std::uint32_t() const;

        explicit operator bool() const;

        std::string to_string() const;

        bignum &operator+=(const bignum &other);

        bignum &operator*=(const bignum &other);

        // swap *this and other
        void swap(bignum &other);

    private:
        // returns a % 10, a /= 10
        std::uint32_t mod();

        // remove leading zeros from bignum
        void remove_leading_zeros();

        /// fields:
        std::vector<std::uint32_t> digs_;
        static const uint64_t base = 4294967296;  // base of radix
    };

    inline bignum::bignum(std::uint32_t pref) : digs_{pref} {
    }

    inline void bignum::remove_leading_zeros() {
        for (auto rit = digs_.rbegin(); rit != digs_.rend() - 1; ++rit) {
            if (*rit != 0)
                break;
            digs_.pop_back();
        }
    }

    inline void bignum::swap(bignum &other) {
        digs_.swap(other.digs_);
    }

    inline  bignum::bignum(const std::string &str) {
        std::size_t begin = 0;
        while (str[begin] == '+') begin++;
        while (str[begin] == '0') begin++;
        std::string s = str.substr(begin, std::string::npos);
        if (s.size() <= 10) {
            uint64_t tmp = std::stoul(s);
            if (tmp < base) {
                digs_ = std::vector{static_cast<std::uint32_t>(tmp)};
                return;
            }
        }
        *this += stoul(s.substr(0, 9));
        auto it = s.begin() + 9;
        const static std::uint32_t max_decimal_len = 1e9; // 9 - max power of 10 in int32_t
        for (; it + 9 < s.end(); it += 9) {
            *this *= max_decimal_len;
            *this += stoul(std::string(it, it + 9));
        }
        if (it != s.end()) {
            *this *= pow(10, s.end() - it);
            *this += stoul(std::string(it, s.end()));
        }
        remove_leading_zeros();
    }

    inline bignum &bignum::operator=(const bignum &other) {
        if (&other == this) {
            return *this;
        }
        bignum tmp(other);
        swap(tmp);
        return *this;
    }

    inline bignum &bignum::operator=(const std::string &s) {
        bignum tmp(s);
        swap(tmp);
        return *this;
    }

    inline bignum::operator std::uint32_t() const {
        return digs_.front();
    }

    inline bignum::operator bool() const {
        return digs_.size() > 1 || digs_[0] != 0;
    }

    // a % 10, a /= 10
    inline std::uint32_t bignum::mod() {
        std::uint64_t carry = 0;
        for (std::size_t i = 0; i < digs_.size(); ++i) {
            std::uint64_t current = digs_[digs_.size() - i - 1] + carry * bignum::base;
            digs_[digs_.size() - i - 1] = current / 10;
            carry = current % 10;
        }
        while (digs_.size() > 1 && digs_.back() == 0) {
            digs_.pop_back();
        }
        if (digs_.size() == 1) {
            *this = bignum(digs_.front());
        }
        remove_leading_zeros();
        return carry;
    }

    inline std::string bignum::to_string() const {
        std::string res;
        auto copy(*this);
        do {
            res.push_back('0' + copy.mod());
        } while (copy);
        return std::string(res.rbegin(), res.rend());
    }

    inline std::ostream &operator<<(std::ostream &os, const bignum &b) {
        return os << b.to_string();
    }

    inline std::istream &operator>>(std::istream &is, bignum &b) {
        std::string s;
        is >> s;
        bignum tmp(s);
        b.swap(tmp);
        return is;
    }

    inline bignum &bignum::operator+=(const bignum &other) {
        uint64_t carry = 0;
        for (std::size_t i = 0;
             carry || i < std::max(digs_.size(), other.digs_.size()); ++i) {
            uint64_t tmp = carry;
            if (i < digs_.size())
                tmp += digs_[i];
            if (i < other.digs_.size()) {
                tmp += other.digs_[i];
            }
            if (i >= digs_.size()) {
                digs_.push_back(0);
            }
            digs_[i] = tmp % bignum::base;
            carry = (tmp - tmp % bignum::base) / bignum::base;
        }
        remove_leading_zeros();
        return *this;
    }

    inline bignum &bignum::operator*=(const bignum &other) {
        if (!(*this) || !other) {
            digs_ = std::vector{0u};
            return *this;
        }
        bignum res;

        res.digs_ = std::vector<std::uint32_t>(digs_.size() + other.digs_.size() + 1, 0);
        for (std::size_t i = 0; i < digs_.size(); i++) {
            uint64_t carry = 0;
            for (std::size_t j = 0; j < other.digs_.size() || carry != 0; j++) {
                uint64_t tmp = static_cast<uint64_t>(digs_[i]) *
                               (j < other.digs_.size() ? other.digs_[j] : 0);
                tmp += res.digs_[i + j] + carry;
                res.digs_[i + j] = tmp % bignum::base;
                carry = tmp / bignum::base;
            }
        }
        digs_ = res.digs_;
        remove_leading_zeros();
        return *this;
    }

    inline bignum operator+(const bignum &lhs, const bignum &rhs) {
        return bignum(lhs) += rhs;
    }

    inline bignum operator*(const bignum &lhs, const bignum &rhs) {
        return bignum(lhs) *= rhs;
    }
}

namespace mp {
    class polynomial {
    public:
        explicit polynomial(std::string s) {
            auto s2 = s;
            std::size_t degree = 0;
            {
                static const std::regex e(R"(x\^(\d+))");
                std::smatch m;
                while (std::regex_search(s2, m, e)) {
                    std::uint32_t pow = std::stoul(m[1]);
                    if (degree < pow) degree = pow;
                    s2 = s2.substr(m.position(0) + m.length(), std::string::npos);
                }
            }
            coefficients.resize(degree + 1);
            static const std::regex e(R"((\d+)?\*?x\^(\d+))");
            std::smatch m;
            while (std::regex_search(s, m, e)) {
                if (!m[2].matched)
                    break;
                std::uint32_t coefficient = m[1].matched ? std::stoul(m[1]) : 1;
                coefficients[std::stoul(m[2])] += coefficient;
                s = s.substr(m.position(0) + m.length(), std::string::npos);
            }
        }

        std::uint32_t &at(std::size_t index) {
            if (index >= coefficients.size())
                coefficients.resize(index + 1, 0);
            return coefficients.at(index);
        }

        std::uint32_t at(std::size_t index) const {
            return coefficients.at(index);
        }

        template<typename T>
        T operator()(T point) const {
            if (coefficients.empty()) {
                return static_cast<T>(0);
            }
            T res = 0;
            for (int i = static_cast<int>(coefficients.size()) - 1; i > 0; --i) {
                res += coefficients[i];
                res *= point;
            }
            res += coefficients[0];
            return res;
        }

    private:
        std::vector<std::uint32_t> coefficients;
    };
}

#endif
