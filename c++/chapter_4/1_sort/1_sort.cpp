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
