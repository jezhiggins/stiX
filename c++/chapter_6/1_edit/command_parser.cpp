#include "command_parser.hpp"
#include "../../lib/regex/char_seq.hpp"
#include <charconv>
#include <queue>

size_t const stiX::command::line_error = -1;
size_t const stiX::command::code_error = '?';

stiX::command const stiX::command::error = { };

bool stiX::operator==(stiX::command const& lhs, stiX::command const& rhs) {
  return lhs.from_index == rhs.from_index &&
    lhs.to_index == rhs.from_index &&
    lhs.code == rhs.code;
}

namespace {
  auto const DOT = '.';
  auto const DOLLAR = '$';
  auto const MINUS = '-';
  auto const PLUS = '+';
  auto const COMMA = ',';
  auto const SEMI_COLON = ';';

  class command_parser {
  public:
    command_parser(std::string_view i, size_t d, size_t l) :
      input(i),
      dot(d),
      last(l) {
    }

    stiX::command parse() {
      parse_line_numbers();

      parse_command_code();

      return command();
    }

    stiX::command command() {
      while(indicies.size() > 2)
        indicies.pop();

      if (is_error())
        return stiX::command::error;

      return {from(), to(), code};
    }

  private:
    void parse_command_code() {
      code = !input.is_eol() ? input_pop() : '\n';

      if (!input.is_eol())
        failed();
    }

    void parse_line_numbers() {
      if (input.is_eol() || !is_index_start())
        return;

      while (is_index_start()) {
        indicies.push(parse_line_number());

        if (is_separator())
          input.advance();
      }
    }

    size_t parse_line_number() {
      auto lhs = parse_index();

      if (!is_operator())
        return lhs;

      auto op = input_pop();

      auto rhs = parse_number();

      if (op == MINUS)
        rhs *= -1;

      return lhs + rhs;
    }

    size_t parse_index() {
      switch (*input) {
        case DOT:
          input.advance();
          return dot;
        case DOLLAR:
          input.advance();
          return last;
        case PLUS:
        case MINUS:
          return dot;
      }

      return parse_number();
    }

    size_t parse_number() {
      auto n = std::string{};
      while (!input.is_eol() && std::isdigit(*input))
        n += input_pop();

      auto num = stiX::command::line_error;
      auto [_, ec] = std::from_chars(n.data(), n.data() + n.length(), num);

      if (ec != std::errc())
        failed();

      return num;
    }

    bool is_index_start() {
      auto c = *input;
      return c == DOT ||
             c == DOLLAR ||
             is_operator() ||
             std::isdigit(c);
    }

    bool is_separator() {
      auto c = *input;
      return c == COMMA || c == SEMI_COLON;
    }

    bool is_operator() {
      auto c = *input;
      return c == PLUS || c == MINUS;
    }

    char input_pop() {
      auto c = *input;
      input.advance();
      return c;
    }

    bool is_error() const {
      return has_failed ||
             is_error(from()) ||
             is_error(to()) ||
             is_error(code);
    }

    static bool is_error(size_t f) { return f == stiX::command::line_error; }

    static bool is_error(char c) { return c == stiX::command::code_error; }

    void failed() { has_failed = true; }

    size_t from() const {
      if (indicies.empty())
        return dot;
      return indicies.front();
    }

    size_t to() const {
      if (indicies.empty())
        return dot;
      return indicies.back();
    }

    stiX::character_sequence input;
    size_t const dot;
    size_t const last;

    std::queue<size_t> indicies;
    char code = stiX::command::code_error;
    bool has_failed = false;
  };
} // namespace

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  auto parser = command_parser(input, dot, last);

  return parser.parse();
}
