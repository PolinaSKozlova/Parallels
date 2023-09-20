#ifndef PARALLELS_GAUSS_H
#define PARALLELS_GAUSS_H

#include <mutex>
#include <thread>
#include <vector>

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
  void GaussMultiThreadedElimination();
  std::vector<double> GaussBackSubstitution();
  void GaussEliminateElement(int lead_row, int target_row);
  void SwapRows(int lead_row);

  VVDouble& GetGaussMatrix() { return gauss_matrix_.GetMatrix(); }

 private:
 bool CheckNull(std::vector <double> & row, int end);
  Matrix gauss_matrix_;
};
};  // namespace Parallels

#endif  // PARALLELS_GAUSS_H
