#ifndef PARALLELS_GAUSS_H
#define PARALLELS_GAUSS_H

#include <mutex>
#include <thread>
#include <vector>

#include "../matrix/matrix.h"

namespace Parallels {
class Gauss {
 public:
  Gauss() = default;

  std::vector<double> RunUsualGauss(const Matrix& matrix);
  std::vector<double> RunParallelGauss(const Matrix& matrix);
  void GaussElimination();
  void GaussMultiThreadedElimination();
  std::vector<double> GaussBackSubstitution();
  void GaussEliminateElement(int lead_row, int target_row);
  void SwapRows(int lead_row);

  VVDouble& GetGaussMatrix() { return gauss_matrix_.GetMatrix(); }

 private:
  bool CheckNull(const std::vector<double>& row, int end);
  bool HasOneSolution(const Matrix& matrix);
  const double kEpsilon = 1e-9;
  Matrix gauss_matrix_;
  std::mutex mutex_;
};

class GaussExecutor {
 public:
  GaussExecutor() = default;
  ~GaussExecutor() = default;
  std::vector<double> Run(const Matrix& matrix, const int iterations) {
    Gauss gauss;
    std::vector<double> result;
    for (int i = 0; i < iterations; ++i) {
      result = gauss.RunUsualGauss(matrix);
    }
    return result;
  }
  std::vector<double> RunParallel(const Matrix& matrix, const int iterations) {
    Gauss gauss;
    std::vector<double> result;
    for (int i = 0; i < iterations; ++i) {
      result = gauss.RunParallelGauss(matrix);
    }
    return result;
  }
};
};  // namespace Parallels

#endif  // PARALLELS_GAUSS_H
