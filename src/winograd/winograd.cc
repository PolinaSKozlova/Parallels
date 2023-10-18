#include "winograd.h"

#include <functional>
#include <future>
#include <thread>

namespace Parallels {
Matrix Winograd::MultiplyMatrices(const Matrix& a, const Matrix& b) {
  if (!CheckSize(a_.GetCols(), b_.GetRows()))
    throw std::invalid_argument("Matrices are not compatible!");
  if (b.GetRows() == 1)
    throw std::invalid_argument(
        "Algorithm does not support one row or one column matrices!");
  SetMatrix(a, b);
  std::vector<double> row_factor = CountRowFactors();
  std::vector<double> column_factor = CountColumnFactors();

  Matrix result_matrix(a_.GetRows(), b_.GetCols());

  CountResultMatrix(result_matrix, row_factor, column_factor, 0,
                    result_matrix.GetRows());
  return result_matrix;
}

Matrix Winograd::MultiplyMatricesInParallels(const Matrix& a, const Matrix& b,
                                             unsigned int threads_amount) {
  if (!CheckSize(a.GetCols(), b.GetRows()))
    throw std::invalid_argument("Matrices are not compatible!");
  SetMatrix(a, b);
  std::vector<double> row_factor = CountRowFactors();
  std::vector<double> column_factor = CountColumnFactors();

  Matrix result_matrix(a_.GetRows(), b_.GetCols());

  std::vector<std::thread> threads((size_t)threads_amount);
  for (unsigned int i = 1; i <= threads_amount; ++i) {
    const int start = (a_.GetRows() * (i - 1)) / threads_amount;
    const int end = (a_.GetRows() * i) / threads_amount;
    threads.at(i - 1) = std::thread([&]() {
      CountResultMatrix(result_matrix, row_factor, column_factor, start, end);
    });
    for (auto& th : threads) {
      if (th.joinable()) th.join();
    }
  }

  return result_matrix;
}

Matrix Winograd::MultiplyMatricesInPipeline(const Matrix& a, const Matrix& b) {
  if (!CheckSize(a.GetCols(), b.GetRows()))
    throw std::invalid_argument("Matrices are not compatible!");
  SetMatrix(a, b);
  std::packaged_task<std::vector<double>()> count_row_factor(
      std::bind(&Winograd::CountRowFactors, this));
  std::packaged_task<std::vector<double>()> count_column_factor(
      std::bind(&Winograd::CountColumnFactors, this));

  std::future<std::vector<double>> row_future = count_row_factor.get_future();
  std::future<std::vector<double>> column_future =
      count_column_factor.get_future();

  std::thread row_thread(std::move(count_row_factor));
  std::thread column_thread(std::move(count_column_factor));

  row_future.wait();
  column_future.wait();

  std::vector<double> row_factor = row_future.get();
  std::vector<double> column_factor = column_future.get();

  row_thread.join();
  column_thread.join();

  Matrix result_matrix(a_.GetRows(), b_.GetCols());
  std::thread result_thread(
      [&row_factor, &column_factor, &result_matrix, this]() {
        CountResultMatrix(result_matrix, row_factor, column_factor, 0,
                          result_matrix.GetRows());
      });
  result_thread.join();

  return result_matrix;
}

void Winograd::SetMatrix(const Matrix& a, const Matrix& b) {
  a_ = a;
  b_ = b;
  half_size_ = a_.GetCols() / 2;
}

std::vector<double> Winograd::CountRowFactors() {
  std::vector<double> row_factor(a_.GetRows());
  for (int i = 0; i < a_.GetRows(); i++) {
    row_factor[i] = a_.GetMatrix()[i][0] * a_.GetMatrix()[i][1];
    for (int j = 1; j < half_size_; ++j) {
      row_factor[i] = row_factor[i] +
                      a_.GetMatrix()[i][2 * j] * a_.GetMatrix()[i][2 * j + 1];
    }
  }
  return row_factor;
}

std::vector<double> Winograd::CountColumnFactors() {
  std::vector<double> column_factor(b_.GetCols());
  for (int i = 0; i < b_.GetCols(); i++) {
    column_factor[i] = b_.GetMatrix()[0][i] * b_.GetMatrix()[1][i];
    for (int j = 1; j < half_size_; ++j) {
      column_factor[i] = column_factor[i] + b_.GetMatrix()[2 * j][i] *
                                                b_.GetMatrix()[2 * j + 1][i];
    }
  }
  return column_factor;
}

void Winograd::CountResultMatrix(Matrix& result_matrix,
                                 std::vector<double> row_factor,
                                 std::vector<double> column_factor,
                                 const int start, const int end) const {
  for (int i = start; i < end; ++i) {
    for (int j = 0; j < b_.GetCols(); ++j) {
      result_matrix.GetMatrix()[i][j] = -row_factor[i] - column_factor[j];
      for (int k = 0; k < half_size_; ++k) {
        result_matrix.GetMatrix()[i][j] +=
            (a_.GetMatrix()[i][2 * k] + b_.GetMatrix()[2 * k + 1][j]) *
            (a_.GetMatrix()[i][2 * k + 1] + b_.GetMatrix()[2 * k][j]);
      }
    }
  }
  if (IsOddMatrix(a_.GetCols())) {
    CountOddRows(result_matrix, start, end);
  }
}

void Winograd::CountOddRows(Matrix& result, const int start,
                            const int end) const {
  for (int i = start; i < end; ++i) {
    for (int j = 0; j < b_.GetCols(); ++j) {
      result.GetMatrix()[i][j] += a_.GetMatrix()[i][a_.GetCols() - 1] *
                                  b_.GetMatrix()[a_.GetCols() - 1][j];
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
  if (2 * half_size_ == a_cols) return false;
  return true;
}
};  // namespace Parallels
