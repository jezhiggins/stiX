#include "command_parser.hpp"
#include "../../lib/regex/char_seq.hpp"
#include <charconv>


size_t const stiX::command::line_error = -1;
size_t const stiX::command::code_error = '?';

stiX::command const stiX::command::error = { };

bool stiX::operator==(stiX::command const& lhs, stiX::command const& rhs) {
  return lhs.from_index == rhs.from_index &&
    lhs.to_index == rhs.from_index &&
    lhs.code == rhs.code;
}

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
    auto i = stiX::character_sequence { input };
    parse_line_numbers(i);

    code = !i.is_eol() ? *i : '\n';

    i.advance();
    if (!i.is_eol())
      code = stiX::command::code_error;

    return command();
  }

private:
  void parse_line_numbers(stiX::character_sequence& number_seq) {
    from = to = parse_line_number(number_seq);

    if (is_separator(number_seq))
      number_seq.advance();

    if (!is_index_start(number_seq))
      return;

    to = parse_line_number(number_seq);
  }

  size_t parse_line_number(stiX::character_sequence& number) {
    auto num = parse_index(number);

    if (is_error(num))
      return num;

    if (!is_operator(number))
      return num;

    auto op = *number;
    number.advance();

    auto num2 = parse_number(number);

    if (is_error(num2))
      return num2;

    if (op == '-')
      num2 *= -1;

    return num + num2;
  }

  size_t parse_index(stiX::character_sequence& number) {
    if (number.is_eol() || !is_index_start(number))
      return dot;

    switch(*number) {
      case '.':
        number.advance();
        return dot;
      case '$':
        number.advance();
        return last;
      case '+':
      case '-':
        return dot;
    }

    return parse_number(number);
  }

  static size_t parse_number(stiX::character_sequence& number) {
    auto n = std::string { };
    for (; !number.is_eol() && std::isdigit(*number); number.advance())
      n += *number;

    auto num = stiX::command::line_error;
    auto [_, ec] = std::from_chars(n.data(), n.data() + n.length(), num);

    return ec == std::errc() ? num : stiX::command::line_error;
  }

  static bool is_index_start(stiX::character_sequence& number) {
    return *number == '.' ||
      *number == '$' ||
      *number == '-' ||
      *number == '+' ||
      std::isdigit(*number);
  }
  static bool is_separator(stiX::character_sequence& number) {
    return *number == ',' || *number == ';';
  }
  static bool is_operator(stiX::character_sequence& number) {
    return *number == '+' || *number == '-';
  }

  stiX::command command() const {
    if (is_error())
      return stiX::command::error;

    return { from, to, code };
  }

  bool is_error() const {
    return is_error(from) ||
      is_error(to) ||
      is_error(code);
  }
  static bool is_error(size_t f) { return f == stiX::command::line_error; }
  static bool is_error(char c) { return c == stiX::command::code_error; }

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
