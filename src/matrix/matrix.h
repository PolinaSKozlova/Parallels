#ifndef PARALLELS_MATRIX_H
#define PARALLELS_MATRIX_H

#include <iostream>
#include <vector>

namespace Parallels {

class Matrix {
 public:
  Matrix(size_t n) : matrix_(n, std::vector<double>(n)) {}
  Matrix(size_t m, size_t n) : matrix_(m, std::vector<double>(n)) {}
  Matrix(const Matrix& other) : matrix_(other.matrix_) {}
  ~Matrix() = default;

  Matrix& operator=(const Matrix& other) {
    matrix_ = other.matrix_;
    return *this;
  }

  // to think: may be array?
  void FillMatrix(const<std::vector<double>>& matrix) {
    size_t index;
    for (size_t i = 0; i < matrix_.size(); ++i) {
      for (size_t j = 0; j < matrix_[i].size(); ++j) {
        matrix_[i][j] = matrix[index++];
      }
    }
  }

  void PrintMatrix() const {
    for (size_t i = 0; i < matrix_.size(); ++i) {
      for (size_t j = 0; j < matrix_[i].size(); ++j) {
        std::cout << matrix_[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }

 private:
  std::vector<std::vector<double>> matrix_;
};
};  // namespace Parallels

#endif  // PARALLELS_MATRIX_H