#include "command_parser.hpp"
#include "../../lib/regex/pattern_matcher.hpp"

auto const line_numbers = stiX::compile_pattern("%[0-9\\.\\$+-,;]*");
std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last);

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  auto numbers = line_numbers.find(input);

  auto [from, to] = parse_line_numbers(input.substr(numbers.from, numbers.to), dot, last);
  input = input.substr(numbers.to);

  auto code = input.size() != 0 ? input.front() : '\n';

  return command(from, to, code);
}

std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last) {
  return std::make_pair(dot, dot);
}
