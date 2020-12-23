#include <ostream>
#include <istream>
#include <stdexcept>
#include "matrices.hpp"

double **init(std::size_t n, std::size_t m) {
    if (n == 0 || m == 0)
        return nullptr;
    double **data_;
    data_ = new double *[n];
    data_[0] = new double[n * m]{0};
    for (std::size_t i = 1; i < n; ++i) {
        data_[i] = data_[i - 1] + m;
    }
    return data_;
}

Matrix::Matrix(std::size_t n, std::size_t m) : n_(n), m_(m), data_(nullptr) {
    data_ = init(n_, m_);
}

Matrix::~Matrix() noexcept {
    if (data_ != nullptr) {
        delete[] data_[0];
    }
    delete[] data_;
    data_ = nullptr;
}

void swap(Matrix& m1, Matrix& m2) noexcept {
    std::swap(m1.data_, m2.data_);
    std::swap(m1.n_, m2.n_);
    std::swap(m1.m_, m2.m_);
}

Matrix::Matrix(const Matrix &other) {
    n_ = other.n_, m_ = other.m_;
    data_ = init(n_, m_);
    for (std::size_t i = 0; i < n_; ++i)
        for (std::size_t j = 0; j < m_; ++j)
            data_[i][j] = other.data_[i][j];
}

std::size_t Matrix::rowsNumber() const noexcept {
    return n_;
}

std::size_t Matrix::colsNumber() const noexcept {
    return m_;
}

std::ostream &Matrix::operator<<(std::ostream &os) const {
    os << n_ << ' ' << m_ << std::endl;
    for (std::size_t i = 0; i < n_; ++i) {
        for (std::size_t j = 0; j < m_; ++j) {
            os << data_[i][j] << ' ';
        }
        os << std::endl;
    }
    return os;
}

std::istream &Matrix::operator>>(std::istream &is) {
    if (!static_cast<bool>(is)) {
        throw std::runtime_error("problems with istream");
    }
    std::size_t n, m;
    is >> n >> m;
    Matrix tmp(n, m);
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < m; ++j) {
            if (!is.eof()) {
                is >> tmp.data_[i][j];
            }
        }
    }
    swap(*this, tmp);
    return is;
}

std::istream &operator>>(std::istream &is, Matrix &m) {
    return m.operator>>(is);
}

std::ostream &operator<<(std::ostream &os, const Matrix &m) {
    return m.operator<<(os);
}

Matrix &Matrix::operator=(const Matrix &other) {
    if (&other == this) {
        return *this;
    }
    Matrix copy(other);
    swap(*this, copy);
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    if (this->m_ != other.m_ ||
        this->n_ != other.n_) {
        throw std::invalid_argument("matrices can not be summed");
    }
    for (std::size_t i = 0; i < n_; ++i) {
        for (std::size_t j = 0; j < m_; ++j) {
            this->data_[i][j] += other.data_[i][j];
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    if (this->m_ != other.n_) {
        throw std::invalid_argument("matrices can not be multiplied");
    }
    Matrix tmp(n_, other.m_);
    for (std::size_t i = 0; i < this->n_; ++i) {
        for (std::size_t j = 0; j < other.m_; ++j) {
            for (std::size_t k = 0; k < this->m_; ++k) {
                tmp.data_[i][j] += this->data_[i][k] * other.data_[k][j];
            }
        }
    }
    swap(tmp, *this);
    return *this;
}
