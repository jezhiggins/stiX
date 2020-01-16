#include "print.hpp"

#include <iostream>

void header(std::string filename, size_t pageNumber, std::ostream& output);
void footer(std::ostream& output);
std::string getline(std::istream &input);

namespace stiX {
    int print(
        std::string filename,
        std::istream& input,
        std::ostream& output,
        size_t pageLength
    ) {
      size_t pageCount = 0;
      size_t lineCount = 0;

      while(input && !input.eof()) {
        if (lineCount == 0) {
          ++pageCount;
          header(filename, pageCount, output);
        }

        std::string line = getline(input);

        output << line << '\n';
        ++lineCount;

        if (lineCount == pageLength) {
          footer(output);
          lineCount = 0;
        }
      }

      if (lineCount != 0) {
        while (lineCount != pageLength) {
          output << '\n';
          ++lineCount;
        }
        footer(output);
      }

      return pageCount;
    }
}

void header(std::string filename, size_t pageNumber, std::ostream& output) {
  output << "\n\n"
         << filename
         << " Page " << pageNumber << '\n'
         << "\n\n";
}

void footer(std::ostream& output) {
  output << "\n\n";
}

std::string getline(std::istream &input) {
  std::string line;
  std::getline(input, line);
  return line;
} // getline

