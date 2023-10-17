#ifndef PARALLELS_WINOGRAD_H
#define PARALLES_WINOGRAD_H

#include "../matrix/matrix.h"

namespace Parallels {

class Winograd {
 public:
  Winograd() = default;
  explicit Winograd(const Matrix& a, const Matrix& b) : a_(a), b_(b) {
    if (!CheckSize(a.GetCols(), b.GetRows()))
      throw std::invalid_argument("Matrices are not compatible!");
  }
  ~Winograd() = default;
  Matrix MultiplyMatrices(const Matrix& a, const Matrix& b);
  Matrix MultiplyMatricesInParallels(const Matrix& a, const Matrix& b,
                                     unsigned int threads_amount);
  Matrix MultiplyMatricesInPipeline(const Matrix& a, const Matrix& b);

 private:
  void SetMatrix(const Matrix& a, const Matrix& b);
  std::vector<double> CountRowFactors();
  std::vector<double> CountColumnFactors();
  void CountResultMatrix(Matrix& result_matrix, std::vector<double> row_factor,
                         std::vector<double> column_factor, int start, int end);
  void CountOddRows(Matrix& result, int start, int end);
  bool CheckSize(const int a_cols, const int b_rows) const noexcept;
  bool IsOddMatrix(const int a_cols) const;
  Matrix a_;
  Matrix b_;
  int half_size_{};
};

class WinogradExecutor {
 public:
  WinogradExecutor() = default;
  ~WinogradExecutor() = default;
  Matrix Run(const Matrix& a, const Matrix& b, const int iterations) const {
    Winograd winograd;
    Matrix result;
    for (int i = 0; i < iterations; ++i) {
      result = winograd.MultiplyMatrices(a, b);
    }
    return result;
  }
  Matrix RunParallels(const Matrix& a, const Matrix& b, const int iterations,
                      unsigned int threads_amount) const {
    Winograd winograd;
    Matrix result;
    for (int i = 0; i < iterations; ++i) {
      result = winograd.MultiplyMatricesInParallels(a, b, threads_amount);
    }
    return result;
  }
  Matrix RunPipeline(const Matrix& a, const Matrix& b,
                     const int iterations) const {
    Winograd winograd;
    Matrix result;
    for (int i = 0; i < iterations; ++i) {
      result = winograd.MultiplyMatricesInPipeline(a, b);
    }
    return result;
  }
};
};  // namespace Parallels

#endif  // PARALLELS_WINOGRAD_H
