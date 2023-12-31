#ifndef PARALLELS_WINOGRAD_H
#define PARALLES_WINOGRAD_H

#include <future>
#include <mutex>
#include <queue>

#include "../matrix/matrix.h"

namespace Parallels {

struct WinogradData {
  WinogradData() = default;
  WinogradData(const Matrix& a, const Matrix& b) : a_(a), b_(b), half_size_(a_.GetCols() / 2) {}
  Matrix a_;
  Matrix b_;
  Matrix result_;
  std::vector<double> row_factor_;
  std::vector<double> column_factor_;
  int half_size_{};
};

class Winograd {
 public:
  Winograd() = default;
  explicit Winograd(const Matrix& a, const Matrix& b) : wd_{a, b}, done_(false) {
    if (!CheckSize(wd_.a_.GetCols(), wd_.b_.GetRows())) {
      throw std::invalid_argument("Matrices are not compatible!");
    }
    if (wd_.b_.GetRows() == 1)
      throw std::invalid_argument(
          "Algorithm does not support one row or one column matrices!");
    SetMatrix(wd_.a_, wd_.b_);
  }
  ~Winograd() = default;
  Matrix MultiplyMatrices();
  Matrix MultiplyMatricesInParallels(unsigned int threads_amount);
  Matrix MultiplyMatricesInPipeline();

  WinogradData GetWinogradData() const { return wd_; }
  Matrix GetResultMatrix() const { return wd_.result_; }

 private:
  void SetMatrix(const Matrix& a, const Matrix& b);
  void CountRowFactors();
  void CountColumnFactors();
  void CountResultMatrix(const int start, const int end);
  void CountOddRows(const int start, const int end);
  bool CheckSize(const int a_cols, const int b_rows) const noexcept;
  bool IsOddMatrix(const int a_cols) const;
  WinogradData wd_{};
  std::queue<int> task_queue_;
  std::mutex queue_mutex_;
  std::condition_variable cv_;
  bool done_{};
};

class WinogradExecutor {
 public:
  WinogradExecutor() = default;
  ~WinogradExecutor() = default;
  Matrix Run(const Matrix& a, const Matrix& b, const int iterations) const {
    Winograd winograd(a, b);

    for (int i = 0; i < iterations; ++i) {
      winograd.MultiplyMatrices();
    }

    return winograd.GetResultMatrix();
  }
  Matrix RunParallels(const Matrix& a, const Matrix& b, const int iterations,
                      unsigned int threads_amount) const {
    Winograd winograd(a, b);

    for (int i = 0; i < iterations; ++i) {
      winograd.MultiplyMatricesInParallels(threads_amount);
    }

    return winograd.GetResultMatrix();
  }
  Matrix RunPipeline(const Matrix& a, const Matrix& b,
                     const int iterations) const {
    Winograd winograd(a, b);

    for (int i = 0; i < iterations; ++i) {
      winograd.MultiplyMatricesInPipeline();
    }

    return winograd.GetResultMatrix();
  }
};
};  // namespace Parallels

#endif  // PARALLELS_WINOGRAD_H
