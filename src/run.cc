#include <iostream>

#include "gauss/gauss.h"
#include "matrix/matrix.h"

int main() {
  Parallels::Matrix matrix(3);
  std::vector<double> a = {2.0, 1.0, -1.0, -3.0, -1.0, 2.0, -2.0, 1.0, 2.0};
  matrix.FillMatrix(a);
  matrix.PrintMatrix();
  std::cout << "==================================" << std::endl;
  std::vector<double> b = {1, 2, 3};
  Parallels::Gauss gauss(matrix, b);

  gauss.GaussElimination();

  matrix.PrintMatrix(gauss.GetGaussMatrix());
  gauss.GaussBackSubstitution();

  return 0;
}