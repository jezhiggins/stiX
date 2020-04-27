#include "print.hpp"
#include "../../lib/getline.h"
#include <iostream>
#include <sstream>

void header(
  std::string const& filename,
  size_t pageNumber,
  std::ostream& output
);
void footer(std::ostream& output);
std::string getline(std::istream& input);
size_t countHeaderLines();
size_t countFooterLines();

size_t const headerLines = countHeaderLines();
size_t const footerLines = countFooterLines();

namespace stiX {
  int print(
    std::string const& filename,
    std::istream& input,
    std::ostream& output,
    size_t pageLength
  ) {
    size_t pageCount = 0;
    size_t lineCount = 0;
    size_t const lastLine = pageLength - footerLines;

    while(input && !input.eof()) {
      if (lineCount == 0) {
        header(filename, ++pageCount, output);
        lineCount += headerLines;
      }

      std::string line = getline(input);
      output << line << '\n';
      ++lineCount;

      if (lineCount == lastLine) {
        footer(output);
        lineCount = 0;
      }
    }

    if (lineCount != 0) {
      for ( ; lineCount != lastLine; ++lineCount)
        output << '\n';
      footer(output);
    }

    return pageCount;
  } // print
} // namespace stiX

void header(
  std::string const& filename,
  size_t pageNumber,
  std::ostream& output
) {
  output << "\n\n"
         << filename
         << " Page " << pageNumber << '\n'
         << "\n\n";
} // header

void footer(std::ostream& output) {
  output << "\n\n";
} // footer

size_t countLines(std::string const& s) {
  return std::count(s.cbegin(), s.cend(), '\n');
} // countLines

size_t countHeaderLines() {
  std::stringstream ss;
  header("", 0, ss);
  return countLines(ss.str());
} // countHeaderLines

size_t countFooterLines() {
  std::stringstream ss;
  footer(ss);
  return countLines(ss.str());
} // countFooterLines

