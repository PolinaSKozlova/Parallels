#include <iostream>

#include "gauss/gauss.h"
#include "matrix/matrix.h"

int main() {
  Parallels::Matrix matrix(3);
  matrix.FillMatrix({{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}});
  matrix.PrintMatrix();
  std::cout << "==================================" << std::endl;
  Parallels::Gauss gauss(matrix, {1, 2, 3});

  gauss.GaussElimination();

  matrix.PrintMatrix(gauss.GetGaussMatrix());

  return 0;
}