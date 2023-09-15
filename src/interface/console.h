#ifndef PARALLELS_CONSOLE_H
#define PARALLELS_CONSOLE_H

#include <functional>
#include <iostream>
#include <map>

// #include "../gauss/gauss.h"
// #include "..matrix/matrix.h"

namespace Parallels {
class Console {
  const std::map<std::string, std::function<void()>> options_{
      {"ant", std::bind(&Console::OptionAnt, this)},
      {"gauss", std::bind(&Console::OptionGauss, this)},
      {"winograd", std::bind(&Console::OptionWinograd, this)},
      {"q", std::bind(&Console::OptionExit, this)}};

 public:
  Console(const Console&) = delete;
  ~Console() = default;
  void ShowConsole(std::string& method_name) {
    auto choise = options_.find(method_name);
    while (true) {
      if (choise != options_.end() && choise->first != "q") {
        choise->second();
      } else {
        break;
      }
      std::cin >> method_name;
    }
  }

  static Console* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = new Console();
    }
    return instance_;
  }

 private:
  Console() = default;

  void OptionAnt() {
    std::cout << "Ant\n";
    // ant_thread_.Run();
  }

  void OptionGauss() {
    std::cout << "Gauss\n";
    // gauss_thread_.Run();
  }

  void OptionWinograd() {
    std::cout << "Winograd\n";
    // winograd_thread_.Run();
  }

  void OptionExit() { std::cout << "\nExit\n"; }

  static Console* instance_;
  //  Ant ant_thread_;
  //   Gauss gauss_thread_;
  //   Winograd winograd_thread_;
};

Console* Console::instance_ = nullptr;
};  // namespace Parallels

#endif  // PARALLELS_CONSOLE_H