
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

std::string stiX::centre_line(std::string const& line_in, size_t char_count, size_t width) {
  auto padding = (width - char_count) / 2;

  auto line = std::string(padding, ' ');
  line.append(line_in);

  return line;
}

namespace {
  constinit std::string start_underline_control = "\x1B[4m";
  constinit std::string end_underline_control = "\x1B[0m";

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

  void insert_underline_control(
    std::string& line,
    size_t& boundary,
    std::string const& control) {
    line.insert(boundary, control);
    boundary += control.size();
  }
  void begin_underlining(std::string& line, size_t& boundary) {
    insert_underline_control(line, boundary, start_underline_control);
  }
  void end_underlining(std::string& line, size_t& boundary) {
    insert_underline_control(line, boundary, end_underline_control);
  }
}

std::string stiX::underline(std::string_view line_in) {
  auto line = std::string { line_in };

  auto boundary = word_start(line, 0);
  while (boundary != line.size()) {
    begin_underlining(line, boundary);

    boundary = word_end(line, boundary);
    end_underlining(line, boundary);

    boundary = word_start(line, boundary);
  }

  return line;
}

std::vector<stiX::word_width> stiX::split_into_words(std::string const& line_in) {
  auto tokens = std::vector<stiX::word_width> { };

  auto boundary = word_start(line_in, 0);
  while (boundary != line_in.size()) {
    auto end = word_end(line_in, boundary);

    tokens.emplace_back(line_in.substr(boundary, end), end-boundary);
    boundary = word_start(line_in, end);
  }

  return tokens;
}
