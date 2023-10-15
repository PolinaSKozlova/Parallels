#ifndef PARALLELS_ACO_H
#define PARALLELS_ACO_H

#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <algorithm>

#include "../matrix/matrix.h"

namespace Parallels
{
    class Ant
    {
    private:
        int _started_from;
        int _position;
        Matrix _distances;
        Matrix _pheromones;
        std::set<int> _to_visit;
        std::vector<int> _route;
        double _alpha;
        double _beta;

    public:
        Ant(int position,
            const Matrix &distances,
            const Matrix &pheromones,
            double alpha = 1.0,
            double beta = 1.0)
            : _started_from(position), _position(position), _distances(distances), _pheromones(pheromones), _alpha(alpha), _beta(beta)
        {

            for (int idx = 0; idx < _distances.GetRows(); ++idx)
            {
                _to_visit.insert(idx);
            }
            _to_visit.erase(position);
            _route.push_back(position);
        }

        std::vector<std::pair<int, double>> get_available_directions()
        {
            std::vector<std::pair<int, double>> available_directions;

            VVDouble distances = _distances.GetMatrix();
            for (int idx = 0; idx < _distances.GetRows(); ++idx)
            {
                double direction = distances[_position][idx];
                if (direction != 0.)
                {
                    available_directions.push_back({idx, direction});
                }
            }
            return available_directions;
        }

        void travel(int next_vertex)
        {
            _position = next_vertex;
            _to_visit.erase(next_vertex);
            _route.push_back(next_vertex);
        }

        bool is_journey_over()
        {
            return _to_visit.empty();
        }

        std::pair<int, double> first_direction()
        {
            std::vector<std::pair<int, double>> available_directions = get_available_directions();
            return available_directions[rand() % available_directions.size()];
        }

        double get_desire(double distance, double pheromone)
        {
            return pow(1.0 / distance, _alpha) * pow(pheromone, _beta);
        }

        double desire_to_probability(double desire, double sum_of_desires)
        {
            return desire / sum_of_desires;
        }

        double pheromone_for_direction(int direction)
        {
            return _pheromones.GetMatrix()[_position][direction];
        }

        std::vector<std::pair<int, double>> filter_not_visited(const std::vector<std::pair<int, double>> &available_directions)
        {
            std::vector<std::pair<int, double>> filtered_directions;
            for (const auto &direction : available_directions)
            {
                if (_to_visit.find(direction.first) != _to_visit.end())
                {
                    filtered_directions.push_back(direction);
                }
            }
            return filtered_directions;
        }

        std::vector<std::pair<int, std::pair<double, double>>> prepeare_roulette(const std::vector<std::pair<int, double>> &directions)
        {
            std::vector<std::pair<int, std::pair<double, double>>> roulette;
            std::vector<std::pair<int, double>> desires_for_directions;

            for (const auto &direction : directions)
            {
                double pheromone = pheromone_for_direction(direction.first);
                double desire = get_desire(direction.second, pheromone);
                desires_for_directions.push_back({direction.first, desire});
            }

            double sum_of_desires = 0.0;
            for (const auto &d : desires_for_directions)
            {
                sum_of_desires += d.second;
            }

            double mark = 0.0;
            for (const auto &desire : desires_for_directions)
            {
                double probability = desire_to_probability(desire.second, sum_of_desires);
                roulette.push_back({desire.first, {mark, mark + probability}});
                mark += probability;
            }

            return roulette;
        }

        int make_decision(const std::vector<std::pair<int, std::pair<double, double>>> &roulette, double dice_roll)
        {
            for (const auto &entry : roulette)
            {
                if (dice_roll >= entry.second.first && dice_roll < entry.second.second)
                {
                    return entry.first;
                }
            }
            return -1;
        }

        int next_direction(bool not_visited = true)
        {
            std::vector<std::pair<int, double>> available_directions = get_available_directions();
            std::vector<std::pair<int, double>> choose_from = available_directions;

            if (not_visited)
            {
                choose_from = filter_not_visited(available_directions);
            }

            if (!choose_from.empty())
            {
                std::vector<std::pair<int, std::pair<double, double>>> roulette = prepeare_roulette(choose_from);
                double dice_roll = static_cast<double>(rand()) / RAND_MAX;
                return make_decision(roulette, dice_roll);
            }
            else
            {
                return -1;
            }
        }

        void one_way_tour()
        {
            while (!is_journey_over())
            {
                int next_vertex = next_direction(true);
                if (next_vertex != -1)
                {
                    travel(next_vertex);
                }
                else
                {
                    next_vertex = next_direction(false);
                    travel(next_vertex);
                }
            }
        }

        std::vector<int> full_tour()
        {
            one_way_tour();
            _to_visit.insert(_started_from);
            one_way_tour();
            return _route;
        }

        double get_route_length()
        {
            VVDouble distances = _distances.GetMatrix();
            if (_route.size() > 1)
            {
                double route_length = 0.0;
                for (int idx = 0; idx < static_cast<int>(_route.size() - 1); ++idx)
                {
                    route_length += distances[_route[idx]][_route[idx + 1]];
                }
                return route_length;
            }
            else
            {
                return 0.0;
            }
        }

        std::vector<std::vector<double>> get_pheromone_additive()
        {

            int n_vertices = _distances.GetCols();
            double Q = static_cast<double>(n_vertices);
            double route_length = get_route_length();
            double additive = Q / route_length;

            VVDouble additives(n_vertices, std::vector<double>(n_vertices, 0.0));
            const std::vector<int> &route = _route;

            for (int idx = 0; idx < static_cast<int>(_route.size() - 1); ++idx)
            {
                additives[route[idx]][route[idx + 1]] += additive;
                additives[route[idx + 1]][route[idx]] += additive;
            }

            return additives;
        }
    };

    class Aco
    {
    private:
        Matrix _distances;
        Matrix _pheromones;
        double _alpha;
        double _beta;
        double _p;
        std::vector<int> _best_route;
        double _best_route_length;
        double _n_epochs_max;
        double _n_epochs_stable;

        void init_pheromone_matrix(Matrix &distances,
                                   double initial_pheromone)
        {
            VVDouble distances_matrix = _distances.GetMatrix();
            for (int i = 0; i < distances.GetRows(); ++i)
            {
                for (int j = 0; j < distances.GetCols(); ++j)
                {
                    if (distances_matrix[i][j] != 0)
                    {
                        _pheromones.GetMatrix()[i][j] = initial_pheromone;
                    }
                }
            }
        }

        Ant CreateAnt(int startPosition)
        {
            Matrix localDistances{_distances};
            Matrix localPheromones{_pheromones};

            return Ant(startPosition,
                       localDistances,
                       localPheromones,
                       _alpha,
                       _beta);
        }

    public:
        struct AntTourResult
        {
            std::vector<int> route;
            double route_length;
            std::vector<std::vector<double>> additive;

            void PrintResult()
            {
                std::cout << route_length << std::endl;
                for (const auto &vertex : route)
                {
                    std::cout << vertex << " ";
                }
                std::cout << std::endl;
                for (const auto &vertex : additive)
                {
                    for (const auto &value : vertex)
                    {
                        std::cout << value << " ";
                    }
                    std::cout << std::endl;
                }
            }
        };

        Aco(Matrix input,
            double initial_pheromone,
            double alpha,
            double beta,
            double pheromone_leftover,
            int n_epochs_max,
            int n_epochs_stable)
            : _distances(input),
              _pheromones(input.GetRows(), input.GetCols()),
              _alpha(alpha),
              _beta(beta),
              _p(pheromone_leftover),
              _best_route_length(0.0),
              _n_epochs_max(n_epochs_max),
              _n_epochs_stable(n_epochs_stable)
        {
            init_pheromone_matrix(_distances, initial_pheromone);
        };

        AntTourResult SendAntToTour(int vertexToStart)
        {
            Ant ant = CreateAnt(vertexToStart);
            std::vector<int> route = ant.full_tour();
            double route_length = ant.get_route_length();
            std::vector<std::vector<double>> additive = ant.get_pheromone_additive();

            AntTourResult result;
            result.route = route;
            result.route_length = route_length;
            result.additive = additive;

            return result;
        }

        std::vector<AntTourResult> SendAntsToTour()
        {
            int nAnts = _distances.GetRows();
            std::vector<AntTourResult> results;

            for (int idx = 0; idx < nAnts; ++idx)
            {
                AntTourResult result = SendAntToTour(idx);
                results.push_back(result);
            }

            return results;
        }

        AntTourResult ReduceTourResults(const std::vector<AntTourResult> &results)
        {
            std::vector<std::vector<double>> additive = results[0].additive;
            std::vector<int> bestRoute = results[0].route;
            double bestRouteLength = results[0].route_length;

            for (size_t i = 1; i < results.size(); ++i)
            {
                if (results[i].route_length < bestRouteLength)
                {
                    bestRoute = results[i].route;
                    bestRouteLength = results[i].route_length;
                }

                for (size_t row = 0; row < additive.size(); ++row)
                {
                    for (size_t col = 0; col < additive[row].size(); ++col)
                    {
                        additive[row][col] += results[i].additive[row][col];
                    }
                }
            }

            AntTourResult result;
            result.route = bestRoute;
            result.route_length = bestRouteLength;
            result.additive = additive;

            return result;
        }

        bool UpdateState(const AntTourResult &result)
        {
            bool isItANewRoute = false;

            auto &pheromonesMatrix = _pheromones.GetMatrix();
            for (size_t row = 0; row < pheromonesMatrix.size(); ++row)
            {
                for (size_t col = 0; col < pheromonesMatrix[row].size(); ++col)
                {
                    pheromonesMatrix[row][col] *= _p;
                    pheromonesMatrix[row][col] += result.additive[row][col];
                }
            }

            if (_best_route_length == 0.0 || result.route_length < _best_route_length)
            {
                _best_route = result.route;
                _best_route_length = result.route_length;
                isItANewRoute = true;
            }

            return isItANewRoute;
        }

        bool ExecuteEpoch()
        {
            std::vector<AntTourResult> results = SendAntsToTour();
            AntTourResult reducedResult = ReduceTourResults(results);
            bool isItANewRoute = UpdateState(reducedResult);

            return isItANewRoute;
        }

        void Execute()
        {
            int n_epochs_max = _n_epochs_max;
            int n_epochs_stable = _n_epochs_stable;
            int counter = 1;
            while (n_epochs_max > 0 && n_epochs_stable > 0)
            {
                std::cout << "Starting epoch #" << counter << std::endl;

                bool is_it_a_new_route = ExecuteEpoch();
                if (!is_it_a_new_route)
                {
                    --n_epochs_stable;
                }
                else
                {
                    n_epochs_stable = _n_epochs_stable;
                }

                ++counter;
                --n_epochs_max;

                std::cout << std::boolalpha << is_it_a_new_route << std::endl;
                std::cout << _best_route_length << std::endl;
                for (const auto &vertex : _best_route)
                {
                    std::cout << vertex << " ";
                }
                std::cout << std::endl;
            }
        }

        void State()
        {
            _distances.PrintMatrix();
            _pheromones.PrintMatrix();
            std::cout << _best_route_length << std::endl;
            for (const auto &vertex : _best_route)
            {
                std::cout << vertex << " ";
            }
            std::cout << std::endl;
        }
    };

}; // namespace Parallels

#endif // PARALLELS_ACO_H