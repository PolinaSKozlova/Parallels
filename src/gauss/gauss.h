#ifndef PARALLELS_GAUSS_H
#define PARALLELS_GAUSS_H

#include <vector>

#include "../matrix/matrix.h"

namespace Parallels {
class Gauss {
 public:
  Gauss(Matrix& matrix, std::vector<double>& b) : gauss_matrix_(matrix) {
    gauss_matrix_.MakeExtendedMatrix(b);
  };

  void GaussElimination();
  std::vector<double> GaussBackSubstitution();

  VVDouble& GetGaussMatrix() { return gauss_matrix_.GetMatrix(); }

 private:
  Matrix gauss_matrix_;
};
};  // namespace Parallels

#endif  // PARALLELS_GAUSS_H
