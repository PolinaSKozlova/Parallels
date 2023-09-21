#ifndef PARALLELS_MATRIX_H
#define PARALLELS_MATRIX_H

#include <iostream>
#include <vector>

namespace Parallels {
using VVDouble = std::vector<std::vector<double>>;

class Matrix {
 public:
  Matrix() = default;
  Matrix(size_t n) : matrix_(n, std::vector<double>(n)), rows_(n), cols_(n) {}
  Matrix(size_t m, size_t n)
      : matrix_(m, std::vector<double>(n)), rows_(m), cols_(n) {}
  Matrix(const Matrix& other) : matrix_(other.matrix_) {}
  Matrix(Matrix&& other) : matrix_(std::move(other.matrix_)) {}
  ~Matrix() = default;

  Matrix& operator=(const Matrix& other) {
    matrix_ = other.matrix_;
    rows_ = other.rows_;
    cols_ = other.cols_;
    return *this;
  }

  Matrix& operator=(Matrix&& other) {
    std::swap(matrix_, other.matrix_);
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
    return *this;
  }

  VVDouble& GetMatrix() { return matrix_; }
  VVDouble GetMatrix() const { return matrix_; }
  int GetRows() const noexcept { return rows_; }
  int GetCols() const noexcept { return cols_; }

  // to think: may be array?
  void FillMatrix(std::vector<double>& matrix) {
    int index{};
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        matrix_[i][j] = matrix[index++];
      }
    }
  }

  void MakeExtendedMatrix(std::vector<double>& vector) {
    ++cols_;
    for (size_t i = 0; i < vector.size(); ++i) {
      matrix_[i].push_back(vector[i]);
    }
  }

  bool CheckZeroRow() const {
    bool zero = false;
    int i = 0;
    while (i < rows_) {
      int counter = 0;
      for (int j = 0; j < cols_ - 1; j++) {
        if (matrix_[i][j] == 0.0) {
          counter++;
        }
        if (counter == cols_ - 1) {
          zero = true;
        }
      }
      i++;
    }
    return zero;
  }

  bool CheckZeroCol() const {
    bool zero = false;
    int j = 0;
    while (j < cols_) {
      int counter = 0;
      for (int i = 0; i < rows_; i++) {
        if (matrix_[i][j] == 0.0) {
          counter++;
        }
        if (counter == rows_) {
          zero = true;
        }
      }
      j++;
    }
    return zero;
  }

  void PrintMatrix() const {
    std::cout << "rows " << rows_ << " cols " << cols_ << std::endl;
    for (int i = 0; i < rows_; ++i) {
      for (int j = 0; j < cols_; ++j) {
        std::cout << matrix_[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

  void PrintMatrix(VVDouble& v) const {
    for (size_t i = 0; i < v.size(); ++i) {
      for (size_t j = 0; j < v[i].size(); ++j) {
        std::cout << v[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

 private:
  VVDouble matrix_;
  int rows_{};
  int cols_{};
};
};  // namespace Parallels

#endif  // PARALLELS_MATRIX_H