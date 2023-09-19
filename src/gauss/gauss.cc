#include "gauss.h"

#include <iostream>

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
  std::vector<double> result(gauss_matrix_.GetMatrix().size());
  gauss_matrix_ = matrix;
  GaussElimination();
  result = GaussBackSubstitution();
  return result;
}

std::vector<double> Gauss::RunParallelGauss(const Matrix& matrix) {
  size_t thread_number{
      std::min((size_t)(std::thread::hardware_concurrency() - 1),
               (size_t)matrix.GetRows())};
  gauss_matrix_ = matrix;
  std::vector<std::thread> threads;
  threads.reserve(thread_number);
  GaussMultiThreadedElimination(threads);
  return GaussBackSubstitution();
}

void Gauss::GaussMultiThreadedElimination(std::vector<std::thread>& threads) {
  for (int i = 0; i < gauss_matrix_.GetRows() - 1; ++i) {
    for (int j = i + 1; j < gauss_matrix_.GetRows(); ++j) {
      threads.emplace_back([i, j, this]() { GaussEliminateElement(i, j); });
    }
  }
  for (auto& th : threads) {
    th.join();
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
  if(gauss_matrix_.GetMatrix()[0][0] == 0){
    SwapRows(0);
  } 

if (!CheckNull(gauss_matrix_.GetMatrix()[lead_row], target_row)){
  double factor = gauss_matrix_.GetMatrix()[target_row][lead_row] /
                  gauss_matrix_.GetMatrix()[lead_row][lead_row];
  for (int j = lead_row; j <= gauss_matrix_.GetRows(); ++j) {
    gauss_matrix_.GetMatrix()[target_row][j] -=
        factor * gauss_matrix_.GetMatrix()[lead_row][j];
  }
  }


};

bool Gauss::CheckNull(std::vector <double> & row, int end){
  for(int i= 0; i< end; i++){
    if(row[i]!=0) return false;
  }
  return true;
}

void Gauss::SwapRows(int lead_row) {
  int next_row{lead_row} ;
  while (next_row < gauss_matrix_.GetRows() && gauss_matrix_.GetMatrix()[next_row][lead_row]== 0) {
    next_row++;
  }
  if(next_row >= gauss_matrix_.GetRows()-1) throw std::runtime_error("no solution");
  std::swap(gauss_matrix_.GetMatrix()[lead_row], gauss_matrix_.GetMatrix()[next_row]);
}

};  // namespace Parallels