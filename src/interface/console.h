#ifndef PARALLELS_CONSOLE_H
#define PARALLELS_CONSOLE_H

#include <chrono>
#include <functional>
#include <iostream>
#include <map>

#include "../gauss/gauss.h"
#include "../matrix/matrix.h"
#include "../winograd/winograd2.h"

namespace Parallels {
class Console {
  const std::map<std::string, std::function<void()>> options_{
      {"ant", std::bind(&Console::OptionAnt, this)},
      {"gauss", std::bind(&Console::OptionGauss, this)},
      {"winograd", std::bind(&Console::OptionWinograd, this)}};

 public:
  Console(const Console&) = delete;
  ~Console() = default;
  void ShowConsole(std::string& method_name) {
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

  void DoAlgorithms(std::string& method_name) {
    std::system("clear");
    auto choise = options_.find(ToLower(method_name));
    if (choise != options_.end()) {
      choise->second();
    } else {
      std::cout << "Method not found\n";
    }
    // ClearInput();
  }

  std::string ToLower(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
  }

  void OptionAnt() {
    std::cout << "Ant Method\n";
    // ant_thread_.Run();
  }

  void OptionGauss() {
    std::cout << "Gauss Method\n";
    try {
      std::cout << "Enter matrix size(rows and cols)\n";
      matrix_ = EnterMatrix();
      double time = CountTime([this]() { CallGaussUsual(); });
      std::cout << "Time usual Gauss: " << time << " ms\n";
      time = CountTime([this]() { CallGaussParallel(); });
      std::cout << "Time parallel Gauss: " << time << "\n";
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void OptionWinograd() {
    std::cout << "Winograd Method\n";
    try {
      std::cout << "Enter matrix A size(rows and cols)\n";
      matrix_ = EnterMatrix();
      std::cout << "Enter matrix B size(rows and cols)\n";
      matrix_w_ = EnterMatrix();
      double time = CountTime([this]() { CallWinogradUsual(); });
      std::cout << "Time usual Winograd: " << time << " ms\n";
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
      std::cout << "Usual Gauss Result\n";
      PrintResultVector(gauss_.RunUsualGauss(matrix_));
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallGaussParallel() {
    try {
      std::cout << "Parallel Gauss Result\n";
      PrintResultVector(gauss_.RunParallelGauss(matrix_));
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradUsual() {
    try {
      // std::cout << "Enter matrix A size(rows and cols)\n";
      // Matrix matrix_a = EnterMatrix();
      // std::cout << "Enter matrix B size(rows and cols)\n";
      // Matrix matrix_b = EnterMatrix();
      // std::cout << "Usual Winograd Result\n";
      winograd_.MultiplyMatrices(matrix_, matrix_w_).PrintMatrix();
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }
  }

  void CallWinogradParallel() {
    try {
      // std::cout << "Enter matrix A size(rows and cols)\n";
      // Matrix matrix_a = EnterMatrix();
      // std::cout << "Enter matrix B size(rows and cols)\n";
      // Matrix matrix_b = EnterMatrix();
      std::cout << "Parallel Winograd Result\n";
      winograd_.MultiplyMatrices(matrix_, matrix_w_).PrintMatrix();
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
};

};  // namespace Parallels

#endif  // PARALLELS_CONSOLE_H
