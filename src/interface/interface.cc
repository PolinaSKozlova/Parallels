#include <iostream>

#include "console.h"

int main(int argc, char* argv[]) {
  std::string method{argv[1]};
  if (argc == 1) {
    Parallels::Console::GetInstance()->ShowConsole(method);
  } else {
    std::cout << "Enter method: ant, gauss or winograd:\n";
    std::cin >> argv[1];
    Parallels::Console::GetInstance()->ShowConsole(method);
  }
  return 0;
}
