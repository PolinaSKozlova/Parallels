#ifndef PARALLELS_CONSOLE_H
#define PARALLELS_CONSOLE_H

#include <chrono>
#include <functional>
#include <iostream>
#include <map>

#include "../gauss/gauss.h"
#include "../matrix/matrix.h"

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
    std::cout << "\nExit\n";
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
    ClearInput();
  }

  std::string ToLower(std::string& s) {
    // s.replace(" ", "");
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
    std::cout << "Enter matrix size(rows and cols)\n";
    try {
      Matrix matrix = EnterMatrix();
      Gauss gauss;
      std::cout << "Usual Gauss Result\n";
      PrintResultVector(gauss.RunUsualGauss(matrix));
      std::cout << "Parallel Gauss Result\n";
      PrintResultVector(gauss.RunParallelGauss(matrix));
    } catch (std::invalid_argument& e) {
      std::cout << e.what() << std::endl;
    }

    // gauss_thread_.Run();
  }

  void OptionWinograd() {
    std::cout << "Winograd Method\n";
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
    ClearInput();
    return result;
  }

  void ClearInput() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
};

};  // namespace Parallels

#endif  // PARALLELS_CONSOLE_H