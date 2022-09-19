#include "command_parser.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include <charconv>

auto const line_numbers = stiX::compile_pattern("%[0-9\\.\\$+-,;]*");
std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last);

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  auto numbers = line_numbers.find(input);

  auto [from, to] = parse_line_numbers(input.substr(numbers.from, numbers.to), dot, last);
  input = input.substr(numbers.to);

  auto code = input.size() != 0 ? input.front() : '\n';

  return command(from, to, code);
}

size_t end_of_number(std::string_view number_input) {
  auto e = number_input.find_first_of(",;");
  return e != std::string_view::npos ? e : number_input.size();
}

size_t end_of_separator(std::string_view number_input) {
  auto e = number_input.find_first_not_of(",;");
  return e != std::string_view::npos ? e : number_input.size();
}

size_t parse_number(std::string_view number, size_t dot, size_t last) {
  if (number.length() == 0)
    return dot;

  if (number.length() == 1)
    switch(number[0]) {
    case '.':
      return dot;
    case '$':
      return last;
  }

  size_t num = -1;
  const auto res = std::from_chars(number.data(),
                                   number.data() + number.size(),
                                   num);
  return num;
}

std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last) {
  auto first_num_len = end_of_number(number_input);
  auto from = parse_number(number_input.substr(0, first_num_len), dot, last);

  return std::make_pair(from, from);
}

