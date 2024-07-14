
#include "format_functions.hpp"
#include <algorithm>
#include <functional>

namespace {
  void fill_to_width(std::string& line, size_t width) {
    auto flip = false;
    auto space = line.find(' ');
    auto line_width = stiX::count_width(line);

    while (line_width != width) {
      line.insert(space, " ");
      ++line_width;

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
  auto padding = (width - count_width(line_in)) / 2;

  auto line = std::string(padding, ' ');
  line.append(line_in);

  return line;
}

namespace {
  constinit char escape_char = '\x1B';
  constinit std::string start_underline_control = "\x1B[4m";
  constinit std::string end_underline_control = "\x1B[0m";
  constinit std::string start_bold_control = "\x1B[1m";
  constinit std::string end_bold_control = "\x1B[0m";
  constinit std::string start_italic_control = "\x1B[3m";
  constinit std::string end_italic_control = "\x1B[0m";
  constinit std::string start_strikethrough_control = "\x1B[9m";
  constinit std::string end_strikethrough_control = "\x1B[0m";

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

  void insert_style_control(
    std::string& line,
    size_t& boundary,
    std::string const& control) {
    line.insert(boundary, control);
    boundary += control.size();
  }

  std::string style(
    std::string const& line_in,
    std::string const& start,
    std::string const& end) {
    auto line = std::string { line_in };

    auto boundary = word_start(line, 0);
    while (boundary != line.size()) {
      insert_style_control(line, boundary, start);

      boundary = word_end(line, boundary);
      insert_style_control(line, boundary, end);

      boundary = word_start(line, boundary);
    }

    return line;
  }
}

std::string stiX::underline(std::string const& line_in) {
  return style(line_in, start_underline_control, end_underline_control);
}
std::string stiX::embolden(std::string const& line_in) {
  return style(line_in, start_bold_control, end_bold_control);
}
std::string stiX::italicise(std::string const& line_in) {
  return style(line_in, start_italic_control, end_italic_control);
}
std::string stiX::strikethrough(const std::string &line_in) {
  return style(line_in, start_strikethrough_control, end_strikethrough_control);
}

size_t stiX::count_width(std::string const& w) {
  size_t l = 0;
  for(auto i = w.begin(); i != w.end(); ++i)
    if (*i == escape_char)
      i += 3;
    else
      ++l;
  return l;
}

std::vector<stiX::word_width> stiX::split_into_words(std::string const& line_in) {
  auto tokens = std::vector<stiX::word_width> { };

  auto boundary = word_start(line_in, 0);
  while (boundary != line_in.size()) {
    auto end = word_end(line_in, boundary);

    auto word = line_in.substr(boundary, end-boundary);
    tokens.emplace_back(word, count_width(word));
    boundary = word_start(line_in, end);
  }

  return tokens;
}
