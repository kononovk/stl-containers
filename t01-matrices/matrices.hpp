#pragma once
#ifndef __MATRICES_HPP_
#define __MATRICES_HPP_

#include <ostream>
#include <istream>
#include <cstdint>

class Matrix {
public:
    Matrix() : n_(0), m_(0), data_(nullptr) {
    }

    Matrix(const Matrix &other);

    Matrix(std::size_t n, std::size_t m);

    ~Matrix() noexcept;

    [[nodiscard]] std::size_t rowsNumber() const noexcept;

    [[nodiscard]] std::size_t colsNumber() const noexcept;

    Matrix& operator+=(const Matrix& other);

    Matrix& operator*=(const Matrix& other);

    Matrix &operator=(const Matrix &another);

    friend std::ostream &operator<<(std::ostream &os, const Matrix &m);

    friend std::istream &operator>>(std::istream &is, Matrix &m);

private:
    friend void swap(Matrix &m1, Matrix &m2) noexcept;

    std::ostream &operator<<(std::ostream &os) const;

    std::istream &operator>>(std::istream &is);

    std::size_t n_ = 0, m_ = 0;
    double **data_ = nullptr;
};


#endif