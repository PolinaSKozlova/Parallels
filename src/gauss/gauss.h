#ifndef PARALLELS_GAUSS_H
#define PARALLELS_GAUSS_H

#include <vector>

#include "../matrix/matrix.h"

namespace Parallels {
class Gauss {
 public:
  Gauss(Matrix& matrix, std::vector<double>& b) : matrix_(matrix) {
    matrix_.push_back(b);
  };

 private:
  Matrix matrix_;
};
};  // namespace Parallels

#endif  // PARALLELS_GAUSS_H
