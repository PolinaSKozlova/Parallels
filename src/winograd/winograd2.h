#ifndef PARALLELS_WINOGRAD_H
#define PARALLES_WINOGRAD_H

#include "../matrix/matrix.h"

namespace Parallels {

class Winograd {
 public:
  Winograd() = default;
  explicit Winograd(int rows, int cols);
  ~Winograd() = default;
  Matrix GetResult() const noexcept;
  Matrix MultiplyMatrices(const Matrix& a, const Matrix& b);

 private:
  void CountRowFactors(const Matrix& a);
  void CountColumnFactors(const Matrix& b);
  void CountResultMatrix(const Matrix& a, const Matrix& b);
  void CountOddRows(const Matrix& a, const Matrix& b);
  void AllocateMemory(VVDouble& r_matrix, int rows, int cols) const;
  bool CheckSize(const int a_cols, const int b_rows) const noexcept;
  bool IsOddMatrix(const int a_cols) const;
  Matrix result_;
  std::vector<double> row_factor_;
  std::vector<double> column_factor_;
  int half_size_{};
};
};  // namespace Parallels

#endif  // PARALLELS_WINOGRAD_H