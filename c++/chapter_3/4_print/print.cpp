#include "print.hpp"

#include <iostream>

std::string getline(std::istream &input);

namespace stiX {
    int print(
        std::string filename,
        std::istream& input,
        std::ostream& output,
        size_t pageLength
    ) {
      size_t pageCount = 0;

      while(input) {

      }

      return pageCount;
    }
}

std::string getline(std::istream &input) {
  std::string line;
  std::getline(input, line);
  return line;
} // getline

