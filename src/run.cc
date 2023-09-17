#include <iostream>

#include "gauss/gauss.h"
#include "matrix/matrix.h"

int main() {
  Parallels::Matrix matrix(4);
  // std::vector<double> a = {2.0, 1.0, -1.0, -3.0, -1.0, 2.0, -2.0, 1.0, 2.0};
  std::vector<double> a = {2.5,  1.5, -1.5, 8.5,  -3.5, -1.5, 2.5, -11.5,
                           -2.5, 1.5, 2.5,  -3.5, 1.5,  2.5,  3.5, 4.5};
  matrix.FillMatrix(a);
  matrix.PrintMatrix();
  std::cout << "==================================" << std::endl;
  std::vector<double> b = {-11, -3, -1, 5};
  Parallels::Gauss gauss(matrix, b);

  for (int i = 1; i < 4; ++i) {
    gauss.GaussColumnElimination(0, i);
  }
  matrix.PrintMatrix(gauss.GetGaussMatrix());
  std::cout << "==================================" << std::endl;
  for (int i = 2; i < 4; ++i) {
    gauss.GaussColumnElimination(1, i);
  }

  // gauss.GaussBackSubstitution();

  return 0;
}