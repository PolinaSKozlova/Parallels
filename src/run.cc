#include "gauss/gauss.h"
#include "matrix/matrix.h"

int main() {
  Parallels::Matrix matrix(3);
  matrix.FillMatrix({{2, 1, -1}, {-3, -1, 2}, {-2, 1, 2}});
  matrix.PrintMatrix();

  return 0;
}