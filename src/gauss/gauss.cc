#include "gauss.h"

#include <iostream>

namespace Parallels {

void Gauss::GaussElimination() {
  for (size_t k = 1; k < gauss_matrix_.GetMatrix().size(); ++k) {
    for (size_t j = k; j < gauss_matrix_.GetMatrix().size(); ++j) {
      double factor = gauss_matrix_.GetMatrix()[j][k - 1] /
                      gauss_matrix_.GetMatrix()[k - 1][k - 1];

      for (size_t i = 0; i < gauss_matrix_.GetMatrix().size() + 1; ++i) {
        gauss_matrix_.GetMatrix()[j][i] -=
            factor * gauss_matrix_.GetMatrix()[k - 1][i];
      }
    }
  }
};

std::vector<double> Gauss::GaussBackSubstitution() {
  std::vector<double> solution(gauss_matrix_.GetMatrix().size());

  for (int i = (int)gauss_matrix_.GetMatrix().size() - 1; i >= 0; --i) {
    solution.at(i) =
        gauss_matrix_.GetMatrix()[i][gauss_matrix_.GetMatrix().size()] /
        gauss_matrix_.GetMatrix()[i][i];

    for (int j = (int)gauss_matrix_.GetMatrix().size() - 1; j > i; --j) {
      solution.at(i) -= gauss_matrix_.GetMatrix()[i][j] * solution.at(j) /
                        gauss_matrix_.GetMatrix()[i][i];
    }
  }

  // std::cout << "solution: " << std::endl;
  // for (size_t i = 0; i < solution.size(); ++i) {
  //   std::cout << solution.at(i) << " ";
  // }
  return solution;
};

void Gauss::GaussColumnElimination(int lead_row, int target_row) {
  double factor = gauss_matrix_.GetMatrix()[ target_row][lead_row] /
                  gauss_matrix_.GetMatrix()[lead_row][lead_row];
  // std::cout << "lead_row: " << lead_row << " target_row: " << target_row
  //           << std::endl;
  // std::cout << "factor: " << factor << "\n "
            // << gauss_matrix_.GetMatrix()[lead_row][lead_row] << " "
            // << gauss_matrix_.GetMatrix()[target_row][lead_row] << std::endl;
  for (size_t j = lead_row; j < gauss_matrix_.GetMatrix().size(); ++j) {
    // std::cout << "j: " << j << std::endl;
    // std::cout << gauss_matrix_.GetMatrix()[target_row][j] << " - "
    //           << factor * gauss_matrix_.GetMatrix()[lead_row][j] << std::endl;
    gauss_matrix_.GetMatrix()[target_row][j] -=
        factor * gauss_matrix_.GetMatrix()[lead_row][j];
    // std::cout << gauss_matrix_.GetMatrix()[target_row][j] << std::endl;
  }
};

};  // namespace Parallels