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

std::ifstream openfile(std::string filename);

int main(int argc, char const* argv[]) {
  auto filenames = stiX::make_arguments(argc, argv);

  for (auto filename : filenames) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Could not open '" << filename << "'\n";
      continue;
    }

    std::copy(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(),
        std::ostreambuf_iterator<char>(std::cout)
    );
  }
}
