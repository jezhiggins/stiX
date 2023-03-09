#include "command_parser.hpp"
#include "lines.hpp"
#include "../../lib/regex/char_seq.hpp"
#include <charconv>
#include <queue>

size_t const stiX::command::line_error = -1;
char const stiX::command::code_error = '?';

stiX::command const stiX::command::error = { };

bool stiX::operator==(stiX::command const& lhs, stiX::command const& rhs) {
  return lhs.from_index == rhs.from_index &&
         lhs.to_index == rhs.to_index &&
         lhs.code == rhs.code;
}

namespace {
  auto const DOT = '.';
  auto const DOLLAR = '$';
  auto const MINUS = '-';
  auto const PLUS = '+';
  auto const COMMA = ',';
  auto const SEMI_COLON = ';';

  stiX::index_fn int_index(size_t index) {
    return [index](stiX::lines const&) { return index; };
  }
  stiX::index_fn add_offset(stiX::index_fn ref_fn, size_t index) {
    return [ref_fn, index](stiX::lines const& buffer) {
      return ref_fn(buffer) + index;
    };
  }

  size_t dot_index_fn(stiX::lines const& buffer) {
    return buffer.dot();
  }
  size_t last_index_fn(stiX::lines const& buffer) {
    return buffer.last();
  }

  size_t line_error_fn(stiX::lines const&) {
    return stiX::command::line_error;
  }

  stiX::parsed_command const parse_error = {
    line_error_fn, line_error_fn, stiX::command::code_error
  };

  class command_parser {
  public:
    command_parser(std::string_view i) :
      input(i) {
    }

    stiX::parsed_command parse() {
      parse_line_numbers();

      parse_command_code();

      return command();
    }

    stiX::parsed_command command() {
      while(indicies.size() > 2)
        indicies.pop();

      if (is_error())
        return parse_error;

      return {from(), to(), code};
    }

  private:
    void parse_command_code() {
      code = !input.is_eol() ? input_pop() : '\n';

      if (!input.is_eol())
        failed();
    }

    void parse_line_numbers() {
      while (is_index_start()) {
        indicies.push(parse_line_number());

        if (is_separator())
          input.advance();
      }
    }

    stiX::index_fn parse_line_number() {
      auto lhs = parse_index();

      if (!is_operator())
        return lhs;

      auto op = input_pop();

      auto rhs = parse_number();

      if (op == MINUS)
        rhs *= -1;

      return add_offset(lhs, rhs);
    }

    stiX::index_fn parse_index() {
      switch (*input) {
        case DOT:
          input.advance();
          return dot_index_fn;
        case DOLLAR:
          input.advance();
          return last_index_fn;
        case PLUS:
        case MINUS:
          return dot_index_fn;
      }

      return int_index(parse_number());
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
             //is_error(from()) ||
             //is_error(to()) ||
             is_error(code);
    }

    static bool is_error(size_t f) {
      return f == stiX::command::line_error;
    }

    static bool is_error(char c) { return c == stiX::command::code_error; }

    void failed() { has_failed = true; }

    stiX::index_fn from() const {
      if (indicies.empty())
        return dot_index_fn;
      return indicies.front();
    }

    stiX::index_fn to() const {
      if (indicies.empty())
        return dot_index_fn;
      return indicies.back();
    }

    stiX::character_sequence input;

    std::queue<stiX::index_fn> indicies;
    char code = stiX::command::code_error;
    bool has_failed = false;
  };

  size_t index_or_error(stiX::index_fn fn, stiX::lines const& buffer) {
    auto index = fn(buffer);
    return (index <= buffer.last()) ? index : stiX::command::line_error;
  }

  bool is_error(size_t from, size_t to, char code) {
    return (from == stiX::command::line_error) ||
           (to == stiX::command::line_error) ||
           (code == stiX::command::code_error);
  }
} // namespace

stiX::parsed_command stiX::parse_command(std::string_view input) {
  auto parser = command_parser(input);

  return parser.parse();
}

stiX::command stiX::parsed_command::compile(stiX::lines const& buffer) const {
  auto from = index_or_error(from_index, buffer);
  auto to = index_or_error(to_index, buffer);

  if (is_error(from, to, code))
    return command::error;
  return { from, to, code };
}

