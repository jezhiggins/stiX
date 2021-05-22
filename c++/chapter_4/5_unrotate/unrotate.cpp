#include "unrotate.hpp"
#include "../../lib/getline.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();
auto const gap = 2;

void stiX::unrotate(std::istream &in, std::ostream &out, size_t line_length, char fold_marker) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);
    out << unrotate_line(line, line_length, fold_marker) << '\n';
  };
}

std::string trim_and_unrotate(std::string const& input_line, size_t fold_position, size_t half_length, size_t output_line_length, char fold_marker);
void copy_and_wrap_left(std::string& output_line, std::string const& input, size_t half_length);
void copy_and_wrap_right(std::string& output_line, std::string const& input, size_t half_length);
std::string trim_trailing_blanks(std::string const& line);

std::string stiX::unrotate_line(std::string const& input_line, size_t output_line_length, char fold_marker) {
  auto fold_position = input_line.find(fold_marker);
  if (fold_position == std::string::npos)
    return input_line;

  auto available_output = output_line_length - gap;
  auto half_length = (output_line_length / 2) - (gap / 2);
  if (input_line.length() > available_output)
    return trim_and_unrotate(input_line, fold_position, half_length, output_line_length, fold_marker);

  auto output_line = std::string(output_line_length, ' ');

  // copy after the fold into first half of the output
  auto after_fold = input_line.substr(fold_position+1);
  copy_and_wrap_left(output_line, after_fold, half_length);

  // copy before the fold into the second half of the output
  auto before_fold = input_line.substr(0, fold_position);
  copy_and_wrap_right(output_line, before_fold, half_length);

  return trim_trailing_blanks(output_line);
}

std::string trim_and_unrotate(std::string const& input_line, size_t fold_position, size_t half_length, size_t output_line_length, char fold_marker)
{
  auto excess = input_line.length() - (half_length * 2);

  auto double_trim = (fold_position >= half_length) && (fold_position <= (input_line.length() - fold_position));

  auto trimmed = double_trim
    ? input_line.substr(0, half_length) + fold_marker + input_line.substr(input_line.length() - (half_length-1))
    : input_line.substr(0, half_length) + input_line.substr(half_length + excess);

  return stiX::unrotate_line(trimmed, output_line_length, fold_marker);
}

void copy_and_wrap_left(std::string& output_line, std::string const& input, size_t half_length) {
  auto output_index = half_length - 1;
  for (auto ai = input.rbegin(); ai != input.rend() && output_index != half_length + 1; ++ai) {
    output_line[output_index] = *ai;
    if (output_index == 0)
      output_index = output_line.length();
    --output_index;
  }
}

void copy_and_wrap_right(std::string& output_line, std::string const& input, size_t half_length) {
  auto output_index = output_line.length() - half_length;
  for (auto bi = input.begin(); bi != input.end() && output_index != half_length - 1; ++bi) {
    output_line[output_index] = *bi;
    ++output_index;
    if (output_index >= output_line.length())
      output_index = 0;
  }
}

std::string trim_trailing_blanks(std::string const& line) {
  auto last_space = std::find_if(
    line.rbegin(),
    line.rend(),
    [](unsigned char ch) {
      return !std::isspace(ch);
    });
  if (last_space == line.rbegin())
    return line;
  return std::string(line.begin(), last_space.base());
}
