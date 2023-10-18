#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <random>

#include "aco/aco.h"

int main()
{
  Parallels::Matrix distances(20);

  std::vector<double> in_distances = {
      0.,
      10.,
      15.,
      20.,
      10.,
      0.,
      35.,
      25.,
      15.,
      35.,
      0.,
      30.,
      20.,
      25.,
      30.,
      0.,
  };

  // matrix generator starts
  const int size = 20;
  std::vector<std::vector<double>> matrix(size, std::vector<double>(size));
  std::vector<double> flattened_matrix;

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(1, 100);

  for (int i = 0; i < size; ++i) {
    for (int j = i + 1; j < size; ++j) {
      double value = static_cast<double>(dist(mt));
      matrix[i][j] = value;
      matrix[j][i] = value;
    }
  }

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      flattened_matrix.push_back(matrix[i][j]);
    }
  }
  // matrix generator ends



  distances.FillMatrix(flattened_matrix);

  Parallels::AcoExecutor aco_executor;

  auto start_time = std::chrono::high_resolution_clock::now();
  std::pair<std::vector<int>, double> res_seq = aco_executor.RunSequential(distances, 10);
  for (const auto &vertex : res_seq.first) {
    std::cout << vertex << " ";
  }
  std::cout << std::endl;
  std::cout << res_seq.second << std::endl;
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
  std::cout << "sequential EXEC TIME: " << duration << " мс" << std::endl;


  start_time = std::chrono::high_resolution_clock::now();
  std::pair<std::vector<int>, double> res_par = aco_executor.RunParallel(distances, 10);
  for (const auto &vertex : res_par.first) {
    std::cout << vertex << " ";
  }
  std::cout << std::endl;
  std::cout << res_par.second << std::endl;
  end_time = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
  std::cout << "parallel EXEC TIME: " << duration << " мс" << std::endl;


  // Parallels::Aco aco{distances, 0.2, 1.0, 1.0, 0.6, 10, 3};
  // aco.State();
  // auto results = aco.SendAntsToTour();

  // auto reduced = aco.ReduceTourResults(results);
  // reduced.PrintResult();
  // aco.Execute();
  // aco.UpdateState(reduced);
  // aco.State();
  // Parallels::Ant ant{0, distances, pheromones};
  // std::vector<int> route = ant.full_tour();

  // for (const auto &vertex : route)
  // {
  //   std::cout << vertex << " ";
  // }

  return 0;
}
