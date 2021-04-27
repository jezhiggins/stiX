//
// sort
//
// sort sorts its input into ascending lexographic order.
// Two lines are in order if they are identical or if the
// leftmost character position in which they differ
// contains characters which are in order, using the
// internal numeric representation of the characters. If
// a line is a proper prefix of another line, it precedes
// that line in sort order.
//
// To print the sorted output of a program
//   program | sort | print
//

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include "../../lib/getline.hpp"

std::vector<std::string> read_lines();
void write_lines(std::vector<std::string> const &lines);

int main() {
  auto lines = read_lines();

  std::sort(lines.begin(), lines.end());

  write_lines(lines);
} // main

std::vector<std::string> read_lines() {
  auto lines = std::vector<std::string> { };

  while (std::cin)
    lines.emplace_back(stiX::getline(std::cin));

  return lines;
} // read_lines

void write_lines(std::vector<std::string> const &lines) {
  std::copy(
    lines.begin(),
    lines.end(),
    std::ostream_iterator<std::string>(std::cout, "\n")
  );
} // write_lines
