#include "command_parser.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include <charconv>

auto const line_numbers = stiX::compile_pattern("%[0-9\\.\\$+-,;]*");
std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last);

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  auto numbers = line_numbers.find(input);

  auto [from, to] = parse_line_numbers(input.substr(numbers.from, numbers.to), dot, last);
  input = input.substr(numbers.to);

  auto code = !input.empty() ? input.front() : '\n';

  return { from, to, code };
}

size_t end_of_number(std::string_view number_input) {
  auto e = number_input.find_first_of(",;");
  return e != std::string_view::npos ? e : number_input.size();
}

size_t end_of_separator(std::string_view number_input) {
  auto e = number_input.find_first_not_of(",;");
  return e != std::string_view::npos ? e : number_input.size();
}

std::pair<size_t, size_t> parse_number(std::string_view number) {
  auto l = 0;
  while (l != number.size() && std::isdigit(number[l]))
    ++l;

  size_t num = -1;
  auto [_, ec] = std::from_chars(number.data(),
                                 number.data() + l,
                                 num);

  return { ec == std::errc() ? num : -1, l };
}

std::pair<size_t, size_t> parse_index(std::string_view number, size_t dot, size_t last) {
  if (number.length() == 0)
    return { dot, 0 };

  switch(number[0]) {
    case '.':
      return {dot, 1};
    case '$':
      return {last, 1};
  }

  return parse_number(number);
}

size_t parse_line_number(std::string_view number, size_t dot, size_t last) {
  auto [num, consumed] = parse_index(number, dot, last);

  if (consumed == number.length())
    return num;

  auto op = number[consumed];

  auto [num2, consumed2] = parse_number(number.substr(++consumed));

  switch(op) {
    case '+':
      break;
    case '-':
      num2 *= -1;
      break;
    default:
      return -1;
  }

  return num + num2;
}


std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last) {
  auto first_num_len = end_of_number(number_input);
  auto from = parse_line_number(number_input.substr(0, first_num_len), dot, last);

  number_input = number_input.substr(first_num_len);
  auto sep_len = end_of_separator(number_input);

  number_input = number_input.substr(sep_len);
  auto second_num_len = end_of_number(number_input);
  auto to = second_num_len
    ? parse_line_number(number_input.substr(0, second_num_len), dot, last)
    : from;

  return { from, to };
}

