#include "unrotate.hpp"
#include "../../lib/getline.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();
auto const gap = 2;

void stiX::unrotate(
  std::istream &in,
  std::ostream &out,
  size_t line_length,
  char fold_marker
) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);
    out << unrotate_line(line, line_length, fold_marker) << '\n';
  };
}

struct output_sizes {
  size_t const full_width;
  size_t const available;
  size_t const half;
  size_t const first_half_begin;
  size_t const first_half_end;
  size_t const first_half_wrap_at;
  size_t const first_half_wrap_to;
  size_t const second_half_begin;
  size_t const second_half_end;
  size_t const second_half_wrap_at;
  size_t const second_half_wrap_to;

  output_sizes(size_t width):
    full_width(width),
    available(width - gap),
    half(available / 2),
    first_half_begin(half-1),
    first_half_end(width-half),
    first_half_wrap_at(0),
    first_half_wrap_to(width),
    second_half_begin(width - half),
    second_half_end(half),
    second_half_wrap_at(full_width),
    second_half_wrap_to(0) {
  }
};

std::string trim_and_unrotate(
  std::string const& input_line,
  size_t fold_position,
  output_sizes const& output_length,
  char fold_marker);
void copy_and_wrap_left(
  std::string& output_line,
  std::string const& input,
  output_sizes const& output_length);
void copy_and_wrap_right(
  std::string& output_line,
  std::string const& input,
  output_sizes const& output_length);
std::string trim_trailing_blanks(std::string const& line);

std::string stiX::unrotate_line(
  std::string const& input_line,
  size_t output_line_length,
  char fold_marker
) {
  auto fold_position = input_line.find(fold_marker);
  if (fold_position == std::string::npos)
    return input_line;

  auto output_length = output_sizes { output_line_length };
  if (input_line.length() > output_length.available)
    return trim_and_unrotate(
      input_line,
      fold_position,
      output_length,
      fold_marker);

  auto output_line = std::string(output_line_length, ' ');

  // copy after the fold into first half of the output
  auto after_fold = input_line.substr(fold_position+1);
  copy_and_wrap_left(output_line, after_fold, output_length);

  // copy before the fold into the second half of the output
  auto before_fold = input_line.substr(0, fold_position);
  copy_and_wrap_right(output_line, before_fold, output_length);

  return trim_trailing_blanks(output_line);
}

std::string trim_and_unrotate(
  std::string const& input_line,
  size_t fold_position,
  output_sizes const& output_length,
  char fold_marker
) {
  auto excess = input_line.length() - output_length.available;

  auto needs_double_trim =
    (fold_position >= output_length.half) &&
    (fold_position <= (input_line.length() - output_length.half));

  auto trimmed = needs_double_trim
    ? input_line.substr(0, output_length.half) +
      fold_marker +
      input_line.substr(input_line.length() - (output_length.half-1))
    : input_line.substr(0, output_length.half) +
      input_line.substr(output_length.half + excess);

  return stiX::unrotate_line(trimmed, output_length.full_width, fold_marker);
}

void copy_and_wrap_left(
  std::string& output_line,
  std::string const& input,
  output_sizes const& output_length
) {
  auto output_index = output_length.first_half_begin;
  for (
    auto ai = input.rbegin();
    ai != input.rend() && output_index != output_length.first_half_end;
    ++ai, --output_index
  ) {
    output_line[output_index] = *ai;
    if (output_index == output_length.first_half_wrap_at)
      output_index = output_length.first_half_wrap_to;
  }
}

void copy_and_wrap_right(
  std::string& output_line,
  std::string const& input,
  output_sizes const& output_length
) {
  auto output_index = output_length.second_half_begin;
  for (
    auto bi = input.begin();
    bi != input.end() && output_index != output_length.second_half_end;
    ++bi
  ) {
    output_line[output_index] = *bi;

    ++output_index;
    if (output_index == output_length.second_half_wrap_at)
      output_index = output_length.second_half_wrap_to;
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
