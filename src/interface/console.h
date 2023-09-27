#ifndef PARALLELS_CONSOLE_H
#define PARALLELS_CONSOLE_H

#include <chrono>
#include <functional>
#include <iostream>
#include <map>

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
    std::cout << "Ant Method\n";
    // ant_thread_.Run();
  }

  void OptionGauss() {
    std::cout << "=============================================\n";
    std::cout << "\t\tGauss Method\n";
    std::cout << "=============================================\n";
    try {
      std::cout << "Enter matrix size(rows and cols)\n";
      matrix_ = EnterMatrix();
      std::cout << "\n=============================================\n";
      double time = CountTime([this]() { CallGaussUsual(); });
      std::cout << "Time usual Gauss: " << time << " ms\n";
      std::cout << "=============================================\n";
      time = CountTime([this]() { CallGaussParallel(); });
      std::cout << "Time parallel Gauss: " << time << "\n";
      std::cout << "=============================================\n";
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void OptionWinograd() {
    std::cout << "=============================================\n";
    std::cout << "\t\tWinograd Method\n";
    std::cout << "=============================================\n";
    try {
      std::cout << "\nEnter matrix A size(rows and cols)\n";
      matrix_ = EnterMatrix();
      std::cout << "\nEnter matrix B size(rows and cols)\n";
      matrix_w_ = EnterMatrix();
      std::cout << "\nEnter amount of iterations\n";
      std::cin >> iterations_;
      std::cout << "\n=============================================\n";
      double time = CountTime([this]() { CallWinogradUsual(); });
      std::cout << "Time usual Winograd: " << time << " ms\n";
      std::cout << "=============================================\n";
      time = CountTime([this]() { CallWinogradParallel(); });
      std::cout << "Time Parallel Winograd: " << time << " ms\n";
      std::cout << "=============================================\n";
      time = CountTime([this]() { CallWinogradConveyor(); });
      std::cout << "Time Convyer Winograd: " << time << " ms\n";
      std::cout << "=============================================\n";
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }

    // winograd_thread_.Run();
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

  void CallGaussUsual() {
    try {
      std::cout << "\t\tUsual Gauss Result\n";
      PrintResultVector(gauss_.RunUsualGauss(matrix_));
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallGaussParallel() {
    try {
      std::cout << "\t\tParallel Gauss Result\n";
      PrintResultVector(gauss_.RunParallelGauss(matrix_));
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradUsual() {
    try {
      std::cout << "\t\tUsual Winograd Result\n";
      winograd_.MultiplyMatrices(matrix_, matrix_w_).PrintMatrix();
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradParallel() {
    try {
      std::cout << "\t\tParallel Winograd Result\n";
      winograd_.MultiplyMatricesInParallels(matrix_, matrix_w_, iterations_)
          .PrintMatrix();
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradConveyor() {
    try {
      std::cout << "\t\tConveyor Winograd Result\n";
      winograd_.MultiplyMatricesInConveyor(matrix_, matrix_w_).PrintMatrix();
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

  // Ant ant;
  Gauss gauss_;
  Winograd winograd_;
  Matrix matrix_;
  Matrix matrix_w_;
  int iterations_{};
};

};  // namespace Parallels

#endif  // PARALLELS_CONSOLE_H
