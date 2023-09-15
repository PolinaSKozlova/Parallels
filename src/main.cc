#include <iostream>

int main(argv[], argc) {
  if (argv.size()) {
    ShowConsole(argv[0]);
  } else {
    ShowConsole();
  }
  return 0;
}