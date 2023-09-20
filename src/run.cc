#include <iostream>

#include "gauss/gauss.h"
#include "matrix/matrix.h"

int main() {
  Parallels::Matrix matrix(12,13);
  // std::vector<double> a = {2.0, 1.0, -1.0, -3.0, -1.0, 2.0, -2.0, 1.0, 2.0};
  // std::vector<double> a = {2.5,  1.5, -1.5, 8.5,  -3.5, -1.5, 2.5, -11.5,
  //                          -2.5, 1.5, 2.5,  -3.5, 1.5,  2.5,  3.5, 4.5};
//   std::vector<double> null_val {0, 0 ,12, -18, 5, 
//                             -2, 0, 3, 5, 0, 
//                            0, 2, 0, 0, 1, 
//                            -4, 8, 12, -6, 13};
// std::vector<double> zero{2.0, 1.0, -1.0, 5.0, 0.0, 0.0, 0.0, -2.0, 1.0, 2.0, 3.0, 4.0};;
std::vector<double> big {1,	1,	1,	1,	1,	1,	1,	 1,	1,	1,	1,	1,	10,
0,	5,	0,	1,	0,	1,	1,	 0,	0,	0,	0,	0,	12,
0,	0,	6,	0,	1,	1,	0,	 0,	0,	0,	1,	0,	12,
0,	0,	0,	3,	0,	1,	1,	 1,	0,	0,	1,	0,	13,
0,	0,	0,	0,	3,	2,	2,	 0,	1,	0,	1,	0,	13,
0,	0,	0,	0,	1,	4,	0,	 1,	1,	0,	1,	0,	14,
0,	0,	0,	0,	1,	0,	4,	 1,	0,	0,	1,	0,	15,
0,	0,	0,	0,	0,	0,	0,	 1,	2,	3,	1,	0,	16,
0,	0,	0,	0,	0,	0,	0,	 0,	2,	1,	4,	0,	17,
0,	0,	0,	0,	0,	0,	0,	 0,	2,	2,	0,	0,	18,
0,	0,	0,	0,	0,	1,	1,	11,	1,	1,	1,	0,	19,
0,	0,	2,	2,	2,	2,	2,	 2,	2,	2,	2,	2,	21};
// Parallels::Matrix matrix2(3,4);
// std::vector<double> d = { 2.5,	 1.5,	-1.5	,    9,
// -3.5,	-1.5,	 2.5,	-11.5,
// -2.5,	 1.5	, 2.5	, -3.5};
  // matrix.FillMatrix(a);
  matrix.FillMatrix(big);
  matrix.PrintMatrix();
  // std::cout << "zero cheks: rows  " << matrix.CheckZeroRow() << " - cols " << matrix.CheckZeroCol() << std::endl;
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


//большая матрица
// 1,	1,	1,	1,	1,	1,	1,	 1,	1,	1,	1,	1,	10,
// 0,	5,	0,	1,	0,	1,	1,	 0,	0,	0,	0,	0,	12,
// 0,	0,	6,	0,	1,	1,	0,	 0,	0,	0,	1,	0,	12,
// 0,	0,	0,	3,	0,	1,	1,	 1,	0,	0,	1,	0,	13,
// 0,	0,	0,	0,	3,	2,	2,	 0,	1,	0,	1,	0,	13,
// 0,	0,	0,	0,	1,	4,	0,	 1,	1,	0,	1,	0,	14,
// 0,	0,	0,	0,	1,	0,	4,	 1,	0,	0,	1,	0,	15,
// 0,	0,	0,	0,	0,	0,	0,	 1,	2,	3,	1,	0,	16,
// 0,	0,	0,	0,	0,	0,	0,	 0,	2,	1,	4,	0,	17,
// 0,	0,	0,	0,	0,	0,	0,	 0,	2,	2,	0,	0,	18,
// 0,	0,	0,	0,	0,	1,	1,	11,	1,	1,	1,	0,	19,
// 0,	0,	2,	2,	2,	2,	2,	 2,	2,	2,	2,	2,	21

  return 0;
}