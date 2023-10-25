#include <iostream>

#include "winograd/winograd.h"

int main() {
  // Parallels::Matrix a(2, 3);
  // Parallels::Matrix b(3, 1);
  // std::vector<double> aa{4, 3, 1, 9, 0, 2};
  // std::vector<double> bb{1, 2, 3};
  Parallels::Matrix a(4, 4);
  Parallels::Matrix b(4, 2);
  std::vector<double> aa{4, 5, 8, 1, -2, 0, 7, 0, 2, 7, 1, 3, 6, 5, 4, 18};
  std::vector<double> bb{1, 3, 5, 4, 6, 7, 8, 2};

  a.FillMatrix(aa);
  b.FillMatrix(bb);
  std::cout << "Matrix a:" << std::endl;
  a.PrintMatrix();
  std::cout << "Matrix b:" << std::endl;
  b.PrintMatrix();
  Parallels::Winograd w(a,b); 
  try {
    w.MultiplyMatrices();
    // Parallels::Matrix res = w.MultiplyMatricesInConveyor(a, b);
    //  w.MultiplyMatricesInParallels(4);
     Parallels::Matrix res = w.GetResultMatrix();
    std::cout << "Matrix res:" << std::endl;
    res.PrintMatrix();
    // std::cout << "getter:" << std::endl;
    // w.GetResult().PrintMatrix();
  } catch (std::invalid_argument &e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}