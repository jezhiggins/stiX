//
// compare
//
// compare file1 file2
//
// compare performs a line-by-line comparison of file1 and
// file2, printing each pair of differing lines, precede by
// a line containing the offending line number and a colon.
// If the files are identical, no output is produced. If one
// file is a prefix of the other, compare reports end of
// file on the shorter file.

#include <iostream>
#include <fstream>
#include "../../lib/arguments.h"
#include "compare.hpp"

std::ifstream openfile(std::string filename);

int main(int argc, char const* argv[]) {
  auto arguments = stiX::make_arguments(argc, argv);
  if (arguments.size() != 2) {
    std::cout << argv[0] << " file1 file2" << std::endl;
    return 0;
  }

  std::ifstream left = openfile(arguments[0]);
  std::ifstream right = openfile(arguments[1]);

  if (!left || !right)
      return 0;

  stiX::compare(
      arguments[0],
      left,
      arguments[1],
      right,
      std::cout
  );
}

std::ifstream openfile(std::string filename) {
    std::ifstream f(filename);
    if (!f)
        std::cout << "Could not open '" << filename << "'\n";
    return f;
}