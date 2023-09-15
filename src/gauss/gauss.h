#ifndef PARALLELS_GAUSS_H
#define PARALLELS_GAUSS_H

#include <vector>

class Gauss {
  using Matrix = std::vector<std::vector<double>>;

 public:
  Gauss() = delete;
  Gauss(size_t n) : matrix_(n, std::vector<double>(n + 1, 0)) {}

  void FillMatrix(const std::vector<std::vector<double>>& matrix) {}

 private:
  Matrix matrix_;
};

#endif  // PARALLELS_GAUSS_H