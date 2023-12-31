#include "winograd.h"

#include <functional>
#include <future>
#include <thread>

namespace Parallels {
Matrix Winograd::MultiplyMatrices() {
  CountRowFactors();
  CountColumnFactors();

  CountResultMatrix(0, wd_.result_.GetRows());

  return GetResultMatrix();
}

Matrix Winograd::MultiplyMatricesInParallels(unsigned int threads_amount) {
  CountRowFactors();
  CountColumnFactors();
  std::mutex mtx;

  std::vector<std::thread> threads((size_t)threads_amount);
  for (unsigned int i = 1; i <= threads_amount; ++i) {
    const int start = (wd_.a_.GetRows() * (i - 1)) / threads_amount;
    const int end = (wd_.a_.GetRows() * i) / threads_amount;
    threads.at(i - 1) = std::move(std::thread([&, start, end]() {
      CountResultMatrix(start, end);
      std::lock_guard<std::mutex> lock(mtx);
    }));
  }
  for (auto& th : threads) {
    if (th.joinable()) th.join();
  }
  return GetResultMatrix();
}

Matrix Winograd::MultiplyMatricesInPipeline() {
  auto factors = [&]() {
    CountRowFactors();
    CountColumnFactors();
    {
      std::lock_guard<std::mutex> lock(queue_mutex_);
      for (int i = 0; i < wd_.a_.GetRows(); i++) {
        task_queue_.push(i);
      }
      done_ = true;
    }
    cv_.notify_all();
  };

  auto result = [&]() {
    while (true) {
      int task{};
      {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        cv_.wait(lock, [this] { return !task_queue_.empty() || done_; });
        if (task_queue_.empty()) break;
        task = task_queue_.front();
        task_queue_.pop();
      }
      CountResultMatrix(task, task + 1);
    }
  };

  std::thread factors_thread(factors);
  std::thread result_thread(result);

  factors_thread.join();
  result_thread.join();

  return GetResultMatrix();
}

void Winograd::SetMatrix(const Matrix& a, const Matrix& b) {
  wd_.a_ = a;
  wd_.b_ = b;
  wd_.result_ = Matrix(wd_.a_.GetRows(), wd_.b_.GetCols());
  wd_.half_size_ = wd_.a_.GetCols() / 2;
}

void Winograd::CountRowFactors() {
  wd_.row_factor_.resize(wd_.a_.GetRows());
  for (int i = 0; i < wd_.a_.GetRows(); i++) {
    wd_.row_factor_[i] = wd_.a_.GetMatrix()[i][0] * wd_.a_.GetMatrix()[i][1];
    for (int j = 1; j < wd_.half_size_; ++j) {
      wd_.row_factor_[i] =
          wd_.row_factor_[i] +
          wd_.a_.GetMatrix()[i][2 * j] * wd_.a_.GetMatrix()[i][2 * j + 1];
    }
  }
}

void Winograd::CountColumnFactors() {
  wd_.column_factor_.resize(wd_.b_.GetCols());
  for (int i = 0; i < wd_.b_.GetCols(); i++) {
    wd_.column_factor_[i] = wd_.b_.GetMatrix()[0][i] * wd_.b_.GetMatrix()[1][i];
    for (int j = 1; j < wd_.half_size_; ++j) {
      wd_.column_factor_[i] =
          wd_.column_factor_[i] +
          wd_.b_.GetMatrix()[2 * j][i] * wd_.b_.GetMatrix()[2 * j + 1][i];
    }
  }
}

void Winograd::CountResultMatrix(const int start, const int end) {
  for (int i = start; i < end; ++i) {
    for (int j = 0; j < wd_.b_.GetCols(); ++j) {
      wd_.result_.GetMatrix()[i][j] =
          -wd_.row_factor_[i] - wd_.column_factor_[j];
      for (int k = 0; k < wd_.half_size_; ++k) {
        wd_.result_.GetMatrix()[i][j] +=
            (wd_.a_.GetMatrix()[i][2 * k] + wd_.b_.GetMatrix()[2 * k + 1][j]) *
            (wd_.a_.GetMatrix()[i][2 * k + 1] + wd_.b_.GetMatrix()[2 * k][j]);
      }
    }
  }

  if (IsOddMatrix(wd_.a_.GetCols())) {
    CountOddRows(start, end);
  }
}

void Winograd::CountOddRows(const int start, const int end) {
  for (int i = start; i < end; ++i) {
    for (int j = 0; j < wd_.b_.GetCols(); ++j) {
      wd_.result_.GetMatrix()[i][j] +=
          wd_.a_.GetMatrix()[i][wd_.a_.GetCols() - 1] *
          wd_.b_.GetMatrix()[wd_.a_.GetCols() - 1][j];
    }
  }
}

bool Winograd::CheckSize(const int a_cols, const int b_rows) const noexcept {
  if (a_cols == b_rows) {
    return true;
  }
  return false;
}

bool Winograd::IsOddMatrix(const int a_cols) const {
  if (2 * wd_.half_size_ == a_cols) return false;
  return true;
}
};  // namespace Parallels
