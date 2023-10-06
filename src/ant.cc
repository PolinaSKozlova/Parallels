#include <iostream>
#include <vector>
#include <set>

#include "aco/aco.h"


int main() {
  Parallels::Matrix distances(4);
  Parallels::Matrix pheromones(4);

  std::vector<double> in_distances = {0., 10., 15., 20., 10., 0., 35., 25., 15., 35., 0., 30., 20., 25., 30., 0.,};
  std::vector<double> in_pheromones = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2,};
  
  distances.FillMatrix(in_distances);
  pheromones.FillMatrix(in_pheromones);




  Parallels::Ant ant{0, distances, pheromones};
  std::vector<int> route = ant.full_tour();

  for (const auto &vertex : route) {
    std::cout << vertex << " ";
  }
    
  return 0;
}