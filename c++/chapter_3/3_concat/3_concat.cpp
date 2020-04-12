//
// concat
//
// concat file ...
//
// concat writes the contents of each of its file arguments
// in turn to its output, thus concatenating them into one
// larger file. Since concat performs no reformatting or
// interpretation of the input files, it is useful for
// displaying the contents of a file

#include <iostream>
#include <fstream>
#include "../../lib/arguments.h"
#include "../../lib/copy.h"

int main(int argc, char const* argv[]) {
  auto filenames = stiX::make_arguments(argc, argv);

  for (auto filename : filenames) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Could not open '" << filename << "'\n";
      continue;
    }

    stiX::copy(file, std::cout);
  }
}
