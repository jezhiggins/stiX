
#include "format_functions.hpp"
#include <algorithm>
#include <functional>

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
  constinit std::string start_underline = "\x1B[4m";
  constinit std::string end_underline = "\x1B[0m";

  size_t find_boundary(std::string const& line, size_t from, auto predicate) {
    while (predicate(line[from]) && from != line.size())
      ++from;
    return from;
  }
  size_t word_start(std::string const& line, size_t from) {
    return find_boundary(line, from, isspace);
  }
  size_t word_end(std::string const& line, size_t from) {
    return find_boundary(line, from, std::not_fn(isspace));
  }
}

std::string stiX::underline(std::string_view line_in) {
  auto line = std::string { line_in };

  auto boundary = word_start(line, 0);
  while (boundary != line.size()) {
    line.insert(boundary, start_underline);
    boundary += start_underline.size();

    boundary = word_end(line, boundary);
    line.insert(boundary, end_underline);
    boundary += end_underline.size();

    boundary = word_start(line, boundary);
  }

  return line;
}