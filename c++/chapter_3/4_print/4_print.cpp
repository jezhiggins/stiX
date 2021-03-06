//
// print
//
// print [ file ... ]
//
// print copies each of its argument files in turn to its
// output, inserting page headers and footers and filling
// last page of each file to full length. A header
// consists of two blank lines, a line giving the filename
// and page number, and two more blank lines; a footer
// consists of two blank lines. Pages for each file are
// numbers starting at one. If no arguments are specified,
// print prints its standard input; the file name is null.
// The text of each file is unmodified - no attempt is made
// to fold line lines or expands tabs to spaces.

#include <iostream>
#include <fstream>
#include "../../lib/arguments.hpp"
#include "print.hpp"

int main(int argc, char const* argv[]) {
  auto filenames = stiX::make_arguments(argc, argv);

  if (filenames.empty()) {
    stiX::print("null", std::cin, std::cout);
    return 0;
  }

  for (auto filename : filenames) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Could not open '" << filename << "'\n";
      continue;
    }

    stiX::print(filename, file, std::cout);
  }
}