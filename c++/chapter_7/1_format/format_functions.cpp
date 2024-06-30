
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

std::string stiX::fill_line(std::string const& line_in, size_t width) {
  auto line = line_in;

  if (line.find(' ') == std::string::npos)
    return line;

  fill_to_width(line, width);

  return line;
}

std::string stiX::centre_line(std::string const& line_in, size_t width) {
  auto padding = (width - line_in.size()) / 2;

  auto line = std::string(padding, ' ');
  line.append(line_in);

  return line;
}

namespace {
  constinit std::string start_underline = "\e[4m";
  constinit std::string end_underline = "\e[0m";

  size_t word_start(std::string const& line, size_t from) {
    while (isspace(line[from]) && from != line.size())
      ++from;
    return from;
  }
  size_t word_end(std::string const& line, size_t from) {
    while (!isspace(line[from]) && from != line.size())
      ++from;
    return from;
  }
}

std::string stiX::underline(std::string const& line_in) {
  auto line = line_in;

  auto boundary = word_start(line, 0);
  while (boundary != line.size()) {
    line.insert(boundary, start_underline);
    boundary += start_underline.size();

    boundary = word_end(line, boundary);
    if (boundary != line.size())
      line.insert(boundary, end_underline);
    else
      line.append(end_underline);
    boundary += end_underline.size();
    boundary = word_start(line, boundary);
  }

  return line;
}