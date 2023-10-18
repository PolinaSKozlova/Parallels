#include "gauss.h"

#include <iostream>
#include <set>

std::mutex mtx_;
namespace Parallels {

// void Gauss::GaussElimination() {
//   for (size_t k = 1; k < gauss_matrix_.GetMatrix().size(); ++k) {
//     for (size_t j = k; j < gauss_matrix_.GetMatrix().size(); ++j) {
//       double factor = gauss_matrix_.GetMatrix()[j][k - 1] /
//                       gauss_matrix_.GetMatrix()[k - 1][k - 1];

//       for (size_t i = 0; i < gauss_matrix_.GetMatrix().size() + 1; ++i) {
//         gauss_matrix_.GetMatrix()[j][i] -=
//             factor * gauss_matrix_.GetMatrix()[k - 1][i];
//       }
//     }
//   }
// };

std::vector<double> Gauss::RunUsualGauss(const Matrix& matrix) {
  if (matrix.CheckZeroRow() || matrix.CheckZeroCol() ||
      !HasOneSolution(matrix)) {
    throw std::invalid_argument("There is no solution!");
  }
  gauss_matrix_ = matrix;
  std::vector<double> result(gauss_matrix_.GetMatrix().size());
  GaussElimination();
  result = GaussBackSubstitution();
  return result;
}

std::vector<double> Gauss::RunParallelGauss(const Matrix& matrix) {
  if (matrix.CheckZeroRow() || matrix.CheckZeroCol() ||
      !HasOneSolution(matrix)) {
    throw std::invalid_argument("There is no solution!");
  }
  gauss_matrix_ = matrix;
  GaussMultiThreadedElimination();
  return GaussBackSubstitution();
}

void Gauss::GaussMultiThreadedElimination() {
   unsigned  int thread_number{
      std::min((std::thread::hardware_concurrency() - 1),
               (unsigned  int)gauss_matrix_.GetRows())};
  std::vector<std::thread> threads(thread_number);

  // size_t all_rows = (gauss_matrix_.GetRows() * (gauss_matrix_.GetRows() -1))/2;
  int i = 0;
  while(i < gauss_matrix_.GetRows()-1){ 
    int j = i+1;
    while( j < gauss_matrix_.GetRows()) {
   size_t t = 0;
  //  if(all_rows < thread_number) {
  //   threads.resize(all_rows);
  //   } 
  //   if(all_rows == 0) {
  //   threads.resize(1);
  //   }
    for( ; t < thread_number &&  j < gauss_matrix_.GetRows(); ++t, ++j) {
      
    threads.at(t) = std::move(std::thread([i, j, this]() { GaussEliminateElement(i, j); }));
   
    // std::cout << "thread id " <<t  << " - "<< threads.at(t).get_id() << std::endl;
    // if(t < thread_number-1)
    //   {  
    //      ++j; }
      } 
      // all_rows -=t;
      t = 0;
 
    for (auto& th : threads) {
      if (th.joinable()) th.join();
    }
  }
  ++i;
  }
};


void Gauss::GaussElimination() {
  for (int i = 0; i < gauss_matrix_.GetRows() - 1; ++i) {
    for (int j = i + 1; j < gauss_matrix_.GetRows(); ++j) {
      GaussEliminateElement(i, j);
    }
  }
};

std::vector<double> Gauss::GaussBackSubstitution() {
  std::vector<double> solution(gauss_matrix_.GetRows());
  for (int i = gauss_matrix_.GetRows() - 1; i >= 0; --i) {
    solution[i] = gauss_matrix_.GetMatrix()[i][gauss_matrix_.GetRows()] /
                  gauss_matrix_.GetMatrix()[i][i];

    for (int j = gauss_matrix_.GetRows() - 1; j > i; --j) {
      solution[i] -= gauss_matrix_.GetMatrix()[i][j] * solution.at(j) /
                     gauss_matrix_.GetMatrix()[i][i];
    }
  }
  return solution;
};

void Gauss::GaussEliminateElement(int lead_row, int target_row) {
  mutex_.lock();
  if (gauss_matrix_.GetMatrix()[lead_row][lead_row] == 0) {
    SwapRows(lead_row);
  }

  if (!CheckNull(gauss_matrix_.GetMatrix()[lead_row], target_row)) {
    double factor = gauss_matrix_.GetMatrix()[target_row][lead_row] /
                    gauss_matrix_.GetMatrix()[lead_row][lead_row];
    for (int j = lead_row; j <= gauss_matrix_.GetRows(); ++j) {
      gauss_matrix_.GetMatrix()[target_row][j] -=
          factor * gauss_matrix_.GetMatrix()[lead_row][j];
      if (std::abs(gauss_matrix_.GetMatrix()[target_row][j]) < kEpsilon) {
        gauss_matrix_.GetMatrix()[target_row][j] = 0.0;
      }
    }
  }
  mutex_.unlock();
};

bool Gauss::CheckNull(const std::vector<double>& row, int end) {
  for (int i = 0; i < end; i++) {
    if (row[i] != 0) return false;
  }
  return true;
}

bool Gauss::HasOneSolution(const Matrix& matrix) {
  return (matrix.GetRows() + 1 == matrix.GetCols());
}

void Gauss::SwapRows(int lead_row) {
  int next_row{lead_row};
  while (next_row < gauss_matrix_.GetRows() &&
         gauss_matrix_.GetMatrix()[next_row][lead_row] == 0) {
    next_row++;
  }
  if (next_row >= gauss_matrix_.GetRows() - 1)
    throw std::runtime_error("no solution");
  std::swap(gauss_matrix_.GetMatrix()[lead_row],
            gauss_matrix_.GetMatrix()[next_row]);
}

};  // namespace Parallels
