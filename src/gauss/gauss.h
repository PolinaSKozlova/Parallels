#ifndef PARALLELS_GAUSS_H
#define PARALLELS_GAUSS_H

#include <vector>
#include <thread>
#include <mutex>

#include "../matrix/matrix.h"

namespace Parallels {
class Gauss {
 public:
  // Gauss(Matrix& matrix, std::vector<double>& b) : gauss_matrix_(matrix) {
  //   // gauss_matrix_.MakeExtendedMatrix(b);
  // };
    // Gauss(Matrix& matrix) : gauss_matrix_(matrix) { };
    Gauss() = default;

std::vector<double> RunUsualGauss(const Matrix& matrix);
std::vector<double> RunParallelGauss(const Matrix& matrix);
  void GaussElimination();
  void GaussMultiThreadedElimination(std::vector<std::thread> &threads);
  std::vector<double> GaussBackSubstitution();
  void GaussEliminateElement(int lead_row, int target_row);

  VVDouble& GetGaussMatrix() { return gauss_matrix_.GetMatrix(); }

 private:
  Matrix gauss_matrix_;

};
};  // namespace Parallels

#endif  // PARALLELS_GAUSS_H
