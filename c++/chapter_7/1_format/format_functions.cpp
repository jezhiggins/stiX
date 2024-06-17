
#include "format_functions.hpp"
#include <algorithm>

namespace {
  void fill_to_width(std::string& line, size_t width) {
    auto flip = false;
    auto space = line.find(' ');

    while (line.size() != width) {
      line.insert(space, " ");
      space = line.find(' ', line.find_first_not_of(' ', space));
      if (space == std::string::npos) {
        std::reverse(line.begin(), line.end());
        flip = !flip;
        space = line.find(' ');
      }
    }
    if (flip)
      std::reverse(line.begin(), line.end());
  }
}

std::string stiX::fill_line(std::string_view line_in, size_t width) {
  auto line = std::string { line_in };

  if (line.find(' ') == std::string::npos)
    return line;

  fill_to_width(line, width);

  return line;
}