#ifndef PARALLELS_CONSOLE_H
#define PARALLELS_CONSOLE_H

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>

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
      std::cout << "Enter method: ant, gauss or winograd or q for exit:\n";
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

  std::string ToLower(std::string& s) const {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
  }

  void OptionAnt() {
    std::cout << "=============================================\n";
    std::cout << "Ant Method\n";
    std::cout << "=============================================\n";
    std::string answer{"yes"};
    while (ToLower(answer) == "yes") {
      std::cout << "Enter matrix size\n";
      int size{};
      std::cin >> size;
      matrix_ = EnterMatrix(size);
      std::cout << "\nEnter amount of iterations\n";
      std::cin >> iterations_;
      if (iterations_ < 0)
        throw std::invalid_argument("Incorrect number of iterations");
      std::cout << "\n=============================================\n";
      // aco_.Execute(matrix_, iterations_);
      std::cout << "Do you want to continue? Enter yes or no\n";
      std::cin >> answer;
    }
  }

  void OptionGauss() {
    std::cout << "=============================================\n";
    std::cout << "\t\tGauss Method\n";
    std::cout << "=============================================\n";
    std::string answer{"yes"};
    while (ToLower(answer) == "yes") {
      try {
        std::cout << "Enter matrix size(rows and cols)\n";
        matrix_ = EnterMatrix();
        std::cout << "\n=============================================\n";
        std::cout << "\nEnter amount of iterations\n";
        std::cin >> iterations_;
        time_ = CountTime([this]() { CallGaussUsual(); });
        std::cout << "Time usual Gauss: " << std::fixed << std::setprecision(6)
                  << time_ << std::defaultfloat << " ms\n";
        std::cout << "=============================================\n";
        time_ = CountTime([this]() { CallGaussParallel(); });
        std::cout << "Time parallel Gauss: " << std::fixed
                  << std::setprecision(6) << time_ << std::defaultfloat << "\n";
        std::cout << "=============================================\n";
      } catch (std::invalid_argument& e) {
        std::cout << "=============================================\n";
        std::cout << e.what() << std::endl;
        std::cout << "=============================================\n";
      } catch (std::runtime_error& e) {
        std::cout << "=============================================\n";
        std::cout << e.what() << std::endl;
        std::cout << "=============================================\n";
      }
      std::cout << "Do you want to continue? Enter yes or no\n";
      std::cin >> answer;
    }
  }

  void OptionWinograd() {
    std::cout << "=============================================\n";
    std::cout << "\t\tWinograd Method\n";
    std::cout << "=============================================\n";
    std::string answer{"yes"};
    while (ToLower(answer) == "yes") {
      try {
        std::cout << "\nEnter matrix A size(rows and cols)\n";
        matrix_ = EnterMatrix();
        std::cout << "\nEnter matrix B size(rows and cols)\n";
        matrix_w_ = EnterMatrix();
        std::cout << "\nEnter amount of iterations\n";
        std::cin >> iterations_;
        if (iterations_ < 0)
          throw std::invalid_argument("Incorrect number of iterations");
        std::cout << "\n=============================================\n";
        time_ = CountTime([this]() { CallWinogradUsual(); });
        std::cout << "Time usual Winograd: " << std::fixed
                  << std::setprecision(6) << time_ << std::defaultfloat
                  << " ms\n";
        std::cout << "=============================================\n";
        time_ = CountTime([this]() { CallWinogradParallel(); });
        std::cout << "Time Parallel Winograd: " << std::fixed
                  << std::setprecision(6) << time_ << std::defaultfloat
                  << " ms\n";
        std::cout << "=============================================\n";
        time_ = CountTime([this]() { CallWinogradPipeline(); });
        std::cout << "Time Convyer Winograd: " << std::fixed
                  << std::setprecision(6) << time_ << std::defaultfloat
                  << " ms\n";
        std::cout << "=============================================\n";
      } catch (std::invalid_argument& e) {
        std::cout << "=============================================\n";
        std::cout << e.what() << std::endl;
        std::cout << "=============================================\n";
      }
      std::cout << "Do you want to continue? Enter yes or no\n";
      std::cin >> answer;
    }
  }

  Matrix EnterMatrix() {
    int rows{}, cols{};
    std::cin >> rows >> cols;
    if (rows < 1 || cols < 1)
      throw std::invalid_argument("Incorrect matrix size");
    std::cout << "Enter matrix value\n";
    Matrix result(rows, cols);
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        std::cin >> result.GetMatrix()[i][j];
      }
    }
    return result;
  }

  Matrix EnterMatrix(int size) {
    if (size < 1) throw std::invalid_argument("Incorrect matrix size");
    std::cout << "Enter matrix value\n";
    Matrix result(size);
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        std::cin >> result.GetMatrix()[i][j];
      }
    }
    return result;
  }

  void CallAntUsual() {
    try {
      std::cout << "\t\tUsual Ant Result\n";
      // PrintResultVector(aco_ex_.Run(matrix_, iterations_));
      aco_ex_.Run(matrix_, iterations_);
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallAntParallel() {
    try {
      std::cout << "\t\tParallel Ant Result\n";
      // PrintResultVector(aco_.Execute());
      // aco_ex_.RunParallel(matrix_, iterations_);
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallGaussUsual() {
    try {
      std::cout << "\t\tUsual Gauss Result\n";
      PrintResultVector(gauss_.Run(matrix_, iterations_));
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallGaussParallel() {
    try {
      std::cout << "\t\tParallel Gauss Result\n";
      PrintResultVector(gauss_.RunParallel(matrix_, iterations_));
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradUsual() {
    try {
      std::cout << "\t\tUsual Winograd Result\n";
      winograd_.Run(matrix_, matrix_w_, iterations_).PrintMatrix();
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradParallel() {
    try {
      std::cout << "\t\tParallel Winograd Result\n";
      int threads{};
      std::cout << "\nEnter amount of threads\n";
      std::cin >> threads;
      winograd_.RunParallels(matrix_, matrix_w_, iterations_, threads)
          .PrintMatrix();
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradPipeline() {
    try {
      std::cout << "\t\tPipeline Winograd Result\n";
      winograd_.RunPipeline(matrix_, matrix_w_, iterations_).PrintMatrix();
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  double CountTime(std::function<void()> f) {
    const auto start{std::chrono::system_clock::now()};
    f();
    const auto finish{std::chrono::system_clock::now()};
    return std::chrono::duration<double>(finish - start).count();
  }

  void PrintResultVector(const std::vector<double>& v) {
    for (auto value : v) {
      std::cout << value << " ";
    }
    std::cout << std::endl;
  }

  AcoExecutor aco_ex_;
  GaussExecutor gauss_;
  WinogradExecutor winograd_;
  Matrix matrix_;
  Matrix matrix_w_;
  int iterations_{};
  double time_{};
};

};  // namespace Parallels

#endif  // PARALLELS_CONSOLE_H
