//
// unique  delete adjacent duplicate lines
//
// unique writes to output only the first line from each
// group of adjacent identical input lines. It is most
// useful for text that has been sorted to bring identical
// lines together; in this case it passes through only
// unique instances of input lines.
//
// To eliminate duplicate lines in the output of a program:
//  program | sort | unique
//
#include "unique.hpp"
#include <iostream>

int main() {
  stiX::unique(std::cin, std::cout);
}