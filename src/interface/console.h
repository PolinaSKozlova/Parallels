#ifndef PARALLELS_CONSOLE_H
#define PARALLELS_CONSOLE_H

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <regex>

#include "../aco/aco.h"
#include "../gauss/gauss.h"
#include "../matrix/matrix.h"
#include "../winograd/winograd.h"

namespace Parallels {
class Console {
  const std::map<std::string, std::function<void()>> options_{
      {"ant", std::bind(&Console::OptionAnt, this)},
      {"gauss", std::bind(&Console::OptionGauss, this)},
      {"winograd", std::bind(&Console::OptionWinograd, this)}};

 public:
  Console(const Console&) = delete;
  ~Console() = default;
  void ShowConsole(std::string& method_name) const {
    while (method_name != "q") {
      if (!method_name.empty()) {
        DoAlgorithms(method_name);
      }
      std::cout << "\n======================================================\n";
      std::cout << "Enter method: ant, gauss or winograd or q for exit:";
      std::cout << "\n======================================================\n";
      std::cin >> method_name;
    }
    std::cout << "Exit\n";
  }

  static Console& GetInstance() {
    static Console instance_;
    return instance_;
  }

 private:
  Console() = default;

  void DoAlgorithms(std::string& method_name) const {
    std::system("clear");
    auto choise = options_.find(ToLower(method_name));
    if (choise != options_.end()) {
      choise->second();
    } else {
      std::cout << "Method not found\n";
    }
  }

  std::string ToLower(std::string s) const {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
  }

  void OptionAnt() {
    ShowMethodName("Ant Method");
    std::string answer{"yes"};
    while (std::regex_match(ToLower(answer), std::regex("yes|y"))) {
      try {
        std::cout << "Enter matrix size\n";
        int size{};
        std::cin >> size;
        ClearInput();
        matrix_ = EnterMatrix(size, true);
        matrix_.PrintToFile("AntMatrixGenerated.txt");
        std::cout << "\n=============================================\n";
        int iterations = GetIterations();
        time_ = CountTime([this, iterations](int) { CallAntUsual(iterations); },
                          iterations);
        ShowTime("Ant usual: ");
        time_ =
            CountTime([this, iterations](int) { CallAntParallel(iterations); },
                      iterations);
        ShowTime("Ant parallel: ");
      } catch (std::invalid_argument& e) {
        std::cout << "=============================================\n";
        std::cout << e.what() << std::endl;
        std::cout << "=============================================\n";
      }
      std::cout << "\n=============================================\n";
      std::cout << "Do you want to continue? Enter yes or no\n";
      std::cin >> answer;
    }
  }

  void OptionGauss() {
    ShowMethodName("Gauss Method");
    std::string answer{"yes"};
    while (std::regex_match(ToLower(answer), std::regex("yes|y"))) {
      try {
        std::cout << "Enter matrix size(rows and cols)\n";
        matrix_ = EnterMatrix();
        matrix_.PrintToFile("GaussMatrixGenerated.txt");
        std::cout << "\n=============================================\n";
        int iterations = GetIterations();
        time_ =
            CountTime([this, iterations](int) { CallGaussUsual(iterations); },
                      iterations);
        ShowTime("Time usual Gauss: ");
        time_ = CountTime(
            [this, iterations](int) { CallGaussParallel(iterations); },
            iterations);
        ShowTime("Time parallel Gauss: ");
        time_ = CountTime(
            [this, iterations](int) { CallGaussParallelStd(iterations); },
            iterations);
        ShowTime("Time parallel Gauss Std: ");
      } catch (std::exception& e) {
        std::cout << "=============================================\n";
        std::cout << e.what() << std::endl;
        std::cout << "=============================================\n";
      }
      std::cout << "\n=============================================\n";
      std::cout << "Do you want to continue? Enter yes or no\n";
      std::cin >> answer;
    }
  }

  void OptionWinograd() {
    ShowMethodName("Winograd Method");
    std::string answer{"yes"};
    while (std::regex_match(ToLower(answer), std::regex("yes|y"))) {
      try {
        std::cout << "\nEnter matrix A size(rows and cols)\n";
        matrix_ = EnterMatrix();
        matrix_.PrintToFile("WinogradMatrixAGenerated.txt");
        std::cout << "\nEnter matrix B size(rows and cols)\n";
        matrix_w_ = EnterMatrix();
        matrix_w_.PrintToFile("WinogradMatrixBGenerated.txt");
        std::cout << "\n=============================================\n";
        int iterations = GetIterations();
        std::cout << "\nEnter amount of threads\n";
        std::cin >> threads_;
        std::cout << "\n=============================================\n";
        time_ = CountTime(
            [this, iterations](int) { CallWinogradUsual(iterations); },
            iterations);
        ShowTime("Time usual Winograd: ");
        time_ = CountTime(
            [this, iterations](int) { CallWinogradParallel(iterations); },
            iterations);
        ShowTime("Time parallel Winograd: ");
        time_ = CountTime(
            [this, iterations](int) { CallWinogradPipeline(iterations); },
            iterations);
        ShowTime("Time pipeline Winograd: ");
      } catch (std::invalid_argument& e) {
        std::cout << "=============================================\n";
        std::cout << e.what() << std::endl;
        std::cout << "=============================================\n";
      }
      std::cout << "Do you want to continue? Enter yes or no\n";
      std::cin >> answer;
    }
  }

  int GetIterations() const {
    std::cout << "Enter amount of iterations\n";
    int iterations{};
    std::cin >> iterations;
    if (iterations < 0)
      throw std::invalid_argument("Incorrect number of iterations");
    return iterations;
  }

  Matrix EnterMatrix(bool graph = false) {
    int rows{}, cols{};
    std::cin >> rows >> cols;
    ClearInput();
    if (rows < 1 || cols < 1)
      throw std::invalid_argument("Incorrect matrix size");
    Matrix result(rows, cols);
    if (std::regex_match(ToLower(CheckAnswer()), std::regex("yes|y"))) {
      if (graph) {
        result.FillRandomMatrixGraph();
      } else {
        result.FillRandomMatrix();
      }
    } else {
      std::cout << "Enter matrix value\n";
      double value{};
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          if (std::cin >> value) {
            result.GetMatrix()[i][j] = value;
          } else {
            ClearInput();
            throw std::invalid_argument("Incorrect matrix value");
          }
        }
      }
      ClearInput();
    }
    return result;
  }

  Matrix EnterMatrix(int size, bool graph = false) {
    if (size < 1) throw std::invalid_argument("Incorrect matrix size");
    Matrix result(size);
    if (std::regex_match(ToLower(CheckAnswer()), std::regex("yes|y"))) {
      if (graph) {
        result.FillRandomMatrixGraph();
      } else {
        result.FillRandomMatrix();
      }
    } else {
      std::cout << "Enter matrix value\n";
      double value{};
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          if (std::cin >> value) {
            result.GetMatrix()[i][j] = value;
          } else {
            ClearInput();
            throw std::invalid_argument("Incorrect matrix value");
          }
        }
      }
    }
    ClearInput();
    return result;
  }

  std::string CheckAnswer() const {
    std::cout << "Do you want create random matrix? Enter yes or no\n";
    std::string answer{};
    std::cin >> answer;
    return answer;
  }

  void CallAntUsual(const int iterations) {
    try {
      std::cout << "\t\tUsual Ant Result\n";
      std::pair<std::vector<int>, double> result =
          aco_ex_.RunSequential(matrix_, iterations);
      std::cout << "Best path: ";
      PrintResultVector(result.first);
      std::cout << "Distance length: " << result.second << std::endl;
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallAntParallel(const int iterations) {
    try {
      std::cout << "\t\tParallel Ant Result\n";
      std::pair<std::vector<int>, double> result =
          aco_ex_.RunParallel(matrix_, iterations);
      std::cout << "Best path: ";
      PrintResultVector(result.first);
      std::cout << "Distance length: " << result.second << std::endl;
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallGaussUsual(const int iterations) {
    try {
      std::cout << "\t\tUsual Gauss Result\n";
      PrintResultVectorToFile(gauss_.Run(matrix_, iterations),
                              "Usual Gauss Result.txt");
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallGaussParallel(const int iterations) {
    try {
      std::cout << "\t\tParallel Gauss Result\n";
      PrintResultVectorToFile(gauss_.RunParallel(matrix_, iterations),
                              "Parallel Gauss Result.txt");
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallGaussParallelStd(const int iterations) {
    try {
      std::cout << "\t\tParallel Gauss Result Std\n";
      PrintResultVectorToFile(gauss_.RunParallelStd(matrix_, iterations),
                              "Parallel Gauss Std Result.txt");
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradUsual(const int iterations) {
    try {
      std::cout << "\t\tUsual Winograd Result\n";
      Matrix result;
      result = winograd_.Run(matrix_, matrix_w_, iterations);
      result.PrintToFile("Usual Winograd Result.txt");
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradParallel(const int iterations) {
    try {
      std::cout << "\t\tParallel Winograd Result\n";
      Matrix result;
      result = winograd_.RunParallels(matrix_, matrix_w_, iterations, threads_);
      result.PrintToFile("Parallel Winograd Result.txt");
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradPipeline(const int iterations) {
    try {
      std::cout << "\t\tPipeline Winograd Result\n";
      Matrix result;
      result = winograd_.RunPipeline(matrix_, matrix_w_, iterations);
      result.PrintToFile("Pipeline Winograd Result.txt");
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  double CountTime(std::function<void(const int)> f, const int iterations) {
    const auto start{std::chrono::system_clock::now()};
    f(iterations);
    const auto finish{std::chrono::system_clock::now()};
    return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start)
        .count();
  }

  void ClearInput() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  void ShowMethodName(const std::string& method_name) const {
    std::cout << "=============================================\n";
    std::cout << "\t\t" << method_name << "\n";
    std::cout << "=============================================\n";
  }

  void ShowTime(const std::string& message) const {
    std::cout << "\n=============================================\n";
    std::cout << message << time_ << " ms \n";
    std::cout << "\n=============================================\n";
  }

  void PrintResultVector(const std::vector<double>& v) const {
    for (auto value : v) {
      std::cout << value << " ";
    }
    std::cout << std::endl;
  }

  void PrintResultVector(const std::vector<int>& v) const {
    for (auto value : v) {
      std::cout << value << " ";
    }
    std::cout << std::endl;
  }

  void PrintResultVectorToFile(const std::vector<double>& v,
                               const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
      int i = 0;
      for (auto value : v) {
        file << "x" << ++i << " = " << value << "\n";
      }
      file.close();
    }
  }

  AcoExecutor aco_ex_;
  GaussExecutor gauss_;
  WinogradExecutor winograd_;
  Matrix matrix_;
  Matrix matrix_w_;
  int iterations_{};
  int threads_{};
  double time_{};
};

};  // namespace Parallels

#endif  // PARALLELS_CONSOLE_H
