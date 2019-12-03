#include <iostream>
#include <fstream>
#include "../../lib/arguments.h"
#include "compare.hpp"

int main(int argc, char const* argv[]) {
  auto arguments = stiX::make_arguments(argc, argv);
  if (arguments.size() != 2) {
    std::cout << argv[0] << " file1 file2" << std::endl;
    return 0;
  }

  std::ifstream left(arguments[0]);
  std::ifstream right(arguments[1]);

  stiX::compare(
      arguments[0],
      left,
      arguments[1],
      right,
      std::cout
  );
}