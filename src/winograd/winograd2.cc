#include "winograd2.h"

namespace Parallels {
Winograd::Winograd(int rows, int cols) : result_(rows, cols) {}

Matrix Winograd::GetResult() const noexcept { return result_; }

Matrix Winograd::MultiplyMatrices(const Matrix& a, const Matrix& b) {
  if (!CheckSize(a.GetCols(), b.GetRows()))
    throw std::invalid_argument("Matrices are not compatible!");
  half_size_ = a.GetCols() / 2;
  CountRowFactors(a);
  CountColumnFactors(b);
  CountResultMatrix(a, b);
  if (IsOddMatrix(a.GetCols())) CountOddRows(a, b);
  return result_;
}

void Winograd::CountRowFactors(const Matrix& a) {
  row_factor_.resize(a.GetRows());
  for (int i = 0; i < a.GetRows(); i++) {
    row_factor_[i] = a.GetMatrix()[i][0] * a.GetMatrix()[i][1];
    for (int j = 1; j < half_size_; ++j) {
      row_factor_[i] = row_factor_[i] +
                       a.GetMatrix()[i][2 * j] * a.GetMatrix()[i][2 * j + 1];
    }
  }
}

void Winograd::CountColumnFactors(const Matrix& b) {
  column_factor_.resize(b.GetRows());
  for (int i = 0; i < b.GetCols(); i++) {
    column_factor_[i] = b.GetMatrix()[0][i] * b.GetMatrix()[1][i];
    for (int j = 1; j < half_size_; ++j) {
      column_factor_[i] = column_factor_[i] +
                          b.GetMatrix()[2 * j][i] * b.GetMatrix()[2 * j + 1][i];
    }
  }
}

void Winograd::CountResultMatrix(const Matrix& a, const Matrix& b) {
  // Parallels::VVDouble r_matrix;
  // AllocateMemory(r_matrix, a.GetRows(), b.GetCols());
  // AllocateMemory(result_.GetMatrix(), a.GetRows(), b.GetCols());
  AllocateMemory(a.GetRows(), b.GetCols());
  for (int i = 0; i < a.GetRows(); ++i) {
    for (int j = 0; j < b.GetCols(); ++j) {
      result_.GetMatrix()[i][j] = -row_factor_[i] - column_factor_[j];
      for (int k = 0; k < half_size_; ++k) {
        result_.GetMatrix()[i][j] =
            result_.GetMatrix()[i][j] +
            (a.GetMatrix()[i][2 * k] + b.GetMatrix()[2 * k + 1][j]) *
                (a.GetMatrix()[i][2 * k + 1] + b.GetMatrix()[2 * k][j]);
      }
    }
  }
  // result_.SetRowsAndCols(a.GetRows(), b.GetCols());
  // result_.SetMatrix(r_matrix);
}

void Winograd::CountOddRows(const Matrix& a, const Matrix& b) {
  for (int i = 0; i < a.GetRows(); ++i) {
    for (int j = 0; j < b.GetCols(); ++j) {
      result_.GetMatrix()[i][j] =
          result_.GetMatrix()[i][j] +
          a.GetMatrix()[i][a.GetCols() - 1] * b.GetMatrix()[a.GetCols() - 1][j];
    }
  }
}

// void Winograd::AllocateMemory(VVDouble& r_matrix, int rows, int cols) {
//   r_matrix.resize(rows);
//   for (int i = 0; i < rows; ++i) {
//     r_matrix[i].resize(cols);
//   }
// }

void Winograd::AllocateMemory(int rows, int cols) {
  Matrix new_matrix(rows, cols);
  result_ = std::move(new_matrix);
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