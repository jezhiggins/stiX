#include "command_parser.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include <charconv>


size_t const stiX::command::line_error = -1;
size_t const stiX::command::code_error = '?';

stiX::command const stiX::command::error = { };

bool stiX::operator==(stiX::command const& lhs, stiX::command const& rhs) {
  return lhs.from_index == rhs.from_index &&
    lhs.to_index == rhs.from_index &&
    lhs.code == rhs.code;
}

auto const line_numbers = stiX::compile_pattern(R"(%[0-9\.\$+-,;]*)");
std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last);
bool is_error(size_t f);
bool is_error(size_t f, size_t t);

class command_parser {
public:
  command_parser(std::string_view i, size_t d, size_t l):
    input(i),
    dot(d),
    last(l)
  {
  }

  stiX::command parse() {
    auto numbers = line_numbers.find(input);

    auto cmd = input.substr(numbers.to);
    if (cmd.length() > 1)
      return stiX::command::error;

    auto [from, to] = parse_line_numbers(input.substr(numbers.from, numbers.to), dot, last);
    if (is_error(from, to))
      return stiX::command::error;

    auto code = !cmd.empty() ? cmd.front() : '\n';
    return { from, to, code };
  }

private:
  std::string_view input;
  size_t const dot;
  size_t const last;
};

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  auto parser = command_parser(input, dot, last);

  return parser.parse();
}

bool is_error(size_t f) {
  return f == stiX::command::line_error;
}
bool is_error(size_t f, size_t t) {
  return is_error(f) || is_error(t);
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

  auto num = stiX::command::line_error;
  auto [_, ec] = std::from_chars(number.data(),
                                 number.data() + l,
                                 num);

  return { ec == std::errc() ? num : stiX::command::line_error, l };
}

std::pair<size_t, size_t> parse_index(std::string_view number, size_t dot, size_t last) {
  if (number.length() == 0)
    return { dot, 0 };

  auto c = number[0];
  switch(c) {
    case '.':
      return {dot, 1};
    case '$':
      return {last, 1};
    default:
      if (!std::isdigit(c))
        return {stiX::command::line_error, 0};
  }

  return parse_number(number);
}

size_t parse_line_number(std::string_view number, size_t dot, size_t last) {
  auto [num, consumed] = parse_index(number, dot, last);

  if (consumed == number.length() || is_error(num))
    return num;

  auto op = number[consumed];

  auto [num2, consumed2] = parse_number(number.substr(++consumed));

  if (is_error(num2))
    return num2;

  switch(op) {
    case '+':
      break;
    case '-':
      num2 *= -1;
      break;
    default:
      return stiX::command::line_error;
  }

  return num + num2;
}


std::pair<size_t, size_t> parse_line_numbers(std::string_view number_input, size_t dot, size_t last) {
  auto first_num_len = end_of_number(number_input);
  auto from = parse_line_number(number_input.substr(0, first_num_len), dot, last);

  if (first_num_len == number_input.length())
    return { from, from };

  number_input = number_input.substr(first_num_len);
  auto sep_len = end_of_separator(number_input);
  if (sep_len == 0) // there number be a separator, if missing then that's an error
    return { stiX::command::line_error, stiX::command::line_error };

  if (number_input.length() == 0)
    return { from, from };

  number_input = number_input.substr(sep_len);
  auto second_num_len = end_of_number(number_input);
  auto to = second_num_len
    ? parse_line_number(number_input.substr(0, second_num_len), dot, last)
    : from;

  return { from, to };
}

