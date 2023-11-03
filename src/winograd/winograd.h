#ifndef PARALLELS_WINOGRAD_H
#define PARALLES_WINOGRAD_H

#include <future>
#include <mutex>
#include <queue>

#include "../matrix/matrix.h"

namespace Parallels {

struct WinogradData {
  WinogradData() = default;
  WinogradData(const Matrix& a, const Matrix& b) : a_(a), b_(b) {}
  Matrix a_;
  Matrix b_;
  Matrix result_;
  std::vector<double> row_factor_;
  std::vector<double> column_factor_;
  int half_size_;
};

class Winograd {
 public:
  Winograd() = default;
  explicit Winograd(const Matrix& a, const Matrix& b) : wd_{a, b} {
    if (!CheckSize(wd_.a_.GetCols(), wd_.b_.GetRows())) {
      throw std::invalid_argument("Matrices are not compatible!");
    }
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
  void CountBothFactors();
  void CountResultMatrix(const int start, const int end);
  void CountResultMatrix(const int i, const int j,
                         std::pair<double, double> factors);
  void CountOddRows(const int start, const int end) const;
  bool CheckSize(const int a_cols, const int b_rows) const noexcept;
  bool IsOddMatrix(const int a_cols) const;
  WinogradData wd_{};
  std::queue<int> task_queue_;  // Очередь задач. Здесь будут храниться индексы
                                // строк для обработки.
  std::mutex
      queue_mutex_;  // Мьютекс для синхронизации доступа к очереди задач.
  std::condition_variable cv_;  // Переменная условия для синхронизации потоков.
  bool done_;  // Флаг, указывающий на завершение вычислений.
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
    Matrix result = winograd.GetResultMatrix();
    return result;
  }
  Matrix RunParallels(const Matrix& a, const Matrix& b, const int iterations,
                      unsigned int threads_amount) const {
    Winograd winograd(a, b);

    for (int i = 0; i < iterations; ++i) {
      winograd.MultiplyMatricesInParallels(threads_amount);
    }
    Matrix result = winograd.GetResultMatrix();
    return result;
  }
  Matrix RunPipeline(const Matrix& a, const Matrix& b,
                     const int iterations) const {
    Winograd winograd(a, b);

    for (int i = 0; i < iterations; ++i) {
      winograd.MultiplyMatricesInPipeline();
    }
    Matrix result = winograd.GetResultMatrix();
    return result;
  }
};
};  // namespace Parallels

#endif  // PARALLELS_WINOGRAD_H
