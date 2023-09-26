#ifndef PARALLELS_WINOGRAD_H
#define PARALLES_WINOGRAD_H

#include <vector>

#include "../matrix/matrix.h"

namespace Parallels {

class Winograd {
 public:
  Winograd() = default;
  ~Winograd() = default;
  Matrix MultiplyMatrices(const Matrix& a, const Matrix& b);
  Matrix MultiplyMatricesInParallels(const Matrix& a, const Matrix& b);
  Matrix MultiplyMatricesInConveyor(const Matrix& a, const Matrix& b);

 private:
  std::vector<double> CountRowFactors(const Matrix& a) const noexcept;
  std::vector<double> CountColumnFactors(const Matrix& b) const noexcept;
  Matrix CountResultMatrix(const Matrix& a, const Matrix& b,
                           std::vector<double> row_factor,
                           std::vector<double> column_factor) const noexcept;
  void CountOddRows(const Matrix& a, const Matrix& b, Matrix& result);
  bool CheckSize(const int a_cols, const int b_rows) const noexcept;
  bool IsOddMatrix(const int a_cols) const noexcept;
  int half_size_{};
};
};  // namespace Parallels

#endif  // PARALLELS_WINOGRAD_H