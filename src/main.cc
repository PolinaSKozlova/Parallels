#include <iostream>

#include "winograd/winograd.h"

int main() {
  Parallels::Matrix a(2, 3);
  Parallels::Matrix b(3, 1);
  std::vector<double> aa{4, 3, 1, 9, 0, 2};
  std::vector<double> bb{1, 2, 3};
  a.FillMatrix(aa);
  b.FillMatrix(bb);
  std::cout << "Matrix a:" << std::endl;
  a.PrintMatrix();
  std::cout << "Matrix b:" << std::endl;
  b.PrintMatrix();
  Parallels::Winograd w;
  try {
    // Parallels::Matrix res = w.MultiplyMatrices(a, b);
    // Parallels::Matrix res = w.MultiplyMatricesInConveyor(a, b);
    Parallels::Matrix res = w.MultiplyMatricesInParallels(a, b, 4);
    std::cout << "Matrix res:" << std::endl;
    res.PrintMatrix();
    // std::cout << "getter:" << std::endl;
    // w.GetResult().PrintMatrix();
  } catch (std::invalid_argument &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}