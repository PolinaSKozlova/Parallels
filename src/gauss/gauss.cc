#include "gauss.h"

#include <iostream>

namespace Parallels {

void Gauss::GaussElimination() {
  for (size_t i = 0; i < gauss_matrix_.GetMatrix().size() - 1; ++i) {
    double factor = gauss_matrix_.GetMatrix()[i][i];
    std::cout << "factor: " << factor << std::endl;
    for (size_t j = i; j < gauss_matrix_.GetMatrix().size(); ++j) {
      gauss_matrix_.GetMatrix()[i][j] /= factor;
      std::cout << "gauss_matrix_[i][j]: " << gauss_matrix_.GetMatrix()[i][j]
                << " ";
    }
    std::cout << std::endl;
    for (size_t j = i + 1; j < gauss_matrix_.GetMatrix().size(); ++j) {
      for (size_t k = i + 1; k < gauss_matrix_.GetMatrix().size(); ++k) {
        gauss_matrix_.GetMatrix()[j][k] -=
            gauss_matrix_.GetMatrix()[i][k] * gauss_matrix_.GetMatrix()[j][i];
      }
    }
  }
};

std::vector<double> Gauss::GaussBackSubstitution() {
  return std::vector<double>();
};
};  // namespace Parallels