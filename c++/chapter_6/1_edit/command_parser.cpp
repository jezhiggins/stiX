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
bool is_error(char c);
bool is_error(size_t f);

size_t end_of_number(std::string_view number_input);
size_t end_of_separator(std::string_view number_input);
size_t parse_line_number(std::string_view number, size_t dot, size_t last);

class command_parser {
public:
  command_parser(std::string_view i, size_t d, size_t l):
    input(i),
    dot(d),
    last(l)
  {
  }

  command_parser(command_parser const&) = delete;

  stiX::command parse() {
    auto numbers = line_numbers.find(input);

    auto cmd = input.substr(numbers.to);

    parse_line_numbers(input.substr(numbers.from, numbers.to));

    if (cmd.length() <= 1)
      code = !cmd.empty() ? cmd.front() : '\n';

    return command();
  }

private:
  void parse_line_numbers(std::string_view number_input) {
    auto first_num_len = end_of_number(number_input);
    from = to = parse_line_number(number_input.substr(0, first_num_len), dot, last);

    if (first_num_len == number_input.length())
      return;

    number_input = number_input.substr(first_num_len);
    auto sep_len = end_of_separator(number_input);
    if (sep_len == 0) // there number be a separator, if missing then that's an error
      return;

    if (number_input.length() == 0)
      return;

    number_input = number_input.substr(sep_len);
    auto second_num_len = end_of_number(number_input);
    to = second_num_len
              ? parse_line_number(number_input.substr(0, second_num_len), dot, last)
              : from;
  }

  stiX::command command() const {
    if (is_error())
      return stiX::command::error;

    return { from, to, code };
  }

  bool is_error() const {
    return ::is_error(from) ||
      ::is_error(to) ||
      ::is_error(code);
  }

  std::string_view input;
  size_t const dot;
  size_t const last;

  size_t from = stiX::command::line_error;
  size_t to = stiX::command::line_error;
  char code = stiX::command::code_error;
};

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  auto parser = command_parser(input, dot, last);

  return parser.parse();
}

bool is_error(char c) { return c == stiX::command::code_error; }
bool is_error(size_t f) { return f == stiX::command::line_error; }

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

