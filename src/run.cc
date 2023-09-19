#include <iostream>

#include "gauss/gauss.h"
#include "matrix/matrix.h"

int main() {
  Parallels::Matrix matrix(4,5);
  // std::vector<double> a = {2.0, 1.0, -1.0, -3.0, -1.0, 2.0, -2.0, 1.0, 2.0};
  // std::vector<double> a = {2.5,  1.5, -1.5, 8.5,  -3.5, -1.5, 2.5, -11.5,
  //                          -2.5, 1.5, 2.5,  -3.5, 1.5,  2.5,  3.5, 4.5};
  std::vector<double> null_val {0, 4 ,12, -18, 5, 
                            -2, 4, 3, 5, 0, 
                           -1, 2, 3, 0, 1, -4, 
                           8, 12, -6, 13};
// Parallels::Matrix matrix2(3,4);
// std::vector<double> d = { 2.5,	 1.5,	-1.5	,    9,
// -3.5,	-1.5,	 2.5,	-11.5,
// -2.5,	 1.5	, 2.5	, -3.5};
  // matrix.FillMatrix(a);
  matrix.FillMatrix(null_val);
  matrix.PrintMatrix();
    // matrix2.FillMatrix(d);
  std::cout << "==================================" << std::endl;
  // std::vector<double> b = {-11, -3, -1, 5};
  // matrix.MakeExtendedMatrix(b);
  Parallels::Gauss gauss;

try{
std::vector<double> solution = gauss.RunParallelGauss(matrix);

// std::vector<double> solution = gauss.RunUsualGauss(matrix);
  matrix.PrintMatrix(gauss.GetGaussMatrix());
  std::cout << "==================================" << std::endl;

  for(size_t i = 0; i < solution.size(); ++i) {
    std::cout << solution[i] << " ";
  }
  std::cout << std::endl;

}catch(std::exception& e){
  std::cout << e.what() << std::endl;
}

  // solution = gauss.RunUsualGauss(matrix2);
  //   std::cout << "==================================" << std::endl;

  // for(size_t i = 0; i < solution.size(); ++i) {
  //   std::cout << solution[i] << " ";
  // }
  // std::cout << std::endl;

// подмена строк
//   0 0 12 -18 5 -9
// -2 4 3 5 0 -7
// -1 2 3 0 1 -4
// -4 8 12 -6 13 -1

  return 0;
}