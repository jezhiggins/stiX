#include "unrotate.hpp"
#include "../../lib/getline.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();
auto const gap = 2;

void stiX::unrotate(std::istream &in, std::ostream &out, size_t line_length, char fold_marker) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);
    out << unrotateLine(line, line_length, fold_marker) << '\n';
  };
}

void copy_and_wrap_left(std::string& output_line, std::string const& input, size_t half_length, size_t excess);
void copy_and_wrap_right(std::string& output_line, std::string const& input, size_t half_length, size_t excess);
std::string trim_trailing_blanks(std::string const& line);


std::string stiX::unrotateLine(std::string const& input_line, size_t output_line_length, char fold_marker) {
  auto fold_position = input_line.find(fold_marker);
  if (fold_position == std::string::npos)
    return input_line;

  auto half_length = (output_line_length / 2) - (gap / 2);
  auto output_line = std::string(output_line_length, ' ');

  auto available_output = output_line_length - gap;
  auto excess = input_line.length() > available_output ? input_line.length() - available_output : 0;

  // copy after the fold into first half of the output
  auto after_fold = input_line.substr(fold_position+1);
  copy_and_wrap_left(output_line, after_fold, half_length, excess);

  // copy before the fold into the second half of the output
  auto before_fold = input_line.substr(0, fold_position);
  copy_and_wrap_right(output_line, before_fold, half_length, excess);

  return trim_trailing_blanks(output_line);
}

void copy_and_wrap_left(std::string& output_line, std::string const& input, size_t half_length, size_t excess) {
  auto output_index = half_length - 1;
  for (auto ai = input.rbegin(); ai != input.rend() && output_index != half_length + 1; ++ai) {
    output_line[output_index] = *ai;
    if (output_index == 0) {
      output_index = output_line.length();
      std::advance(ai, excess);
    }
    --output_index;
  }
}

void copy_and_wrap_right(std::string& output_line, std::string const& input, size_t half_length, size_t excess) {
  auto output_index = output_line.length() - half_length;
  for (auto bi = input.begin(); bi != input.end() && output_index != half_length - 1; ++bi) {
    output_line[output_index] = *bi;
    ++output_index;
    if (output_index >= output_line.length()) {
      // wrapping
      output_index = 0;
      std::advance(bi, excess);
    }
  }
}

std::string trim_trailing_blanks(std::string const& line) {
  auto last_space = std::find_if(
    line.rbegin(),
    line.rend(),
    [](unsigned char ch) {
      return !std::isspace(ch);
    }).base();
  return std::string(line.begin(), last_space);
}
