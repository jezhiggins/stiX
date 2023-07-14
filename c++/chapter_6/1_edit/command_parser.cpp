#include "command_parser.hpp"
#include "lines.hpp"
#include <charconv>
#include <queue>
#include "../../lib/regex/pattern_matcher.hpp"
#include "../../lib/regex/char_seq.hpp"

using namespace std::string_literals;

size_t const stiX::command::line_error = -1;
char const stiX::command::code_error = '?';

stiX::command const stiX::command::error = { };

bool stiX::operator==(command const& lhs, command const& rhs) {
  return lhs.from_index == rhs.from_index &&
         lhs.to_index == rhs.to_index &&
         lhs.code == rhs.code &&
         lhs.filename == rhs.filename;
}

bool stiX::operator!=(command const& lhs, command const& rhs) {
  return !(lhs == rhs);
}

namespace {
  auto const DOT = '.';
  auto const DOLLAR = '$';
  auto const MINUS = '-';
  auto const PLUS = '+';
  auto const SLASH = '/';
  auto const BACKSLASH = '\\';
  auto const COMMA = ',';
  auto const SEMI_COLON = ';';

  bool is_error(char c) { return c == stiX::command::code_error; }
  bool is_error(size_t i) { return i == stiX::command::line_error; }

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

  stiX::index_fn search(
    std::string_view pattern,
    size_t(next_index)(size_t, stiX::lines const&)
  ) {
    auto matcher = stiX::compile_pattern(pattern);
    return [matcher, next_index](stiX::lines const& buffer) {
      size_t index = buffer.dot();
      do {
        index = next_index(index, buffer);

        if (matcher.match(buffer.line_at(index)))
          return index;
      } while (index != buffer.dot());

      return stiX::command::line_error;
    };
  }

  size_t next_line(size_t i, stiX::lines const& buffer) {
    return (i < buffer.last()) ? ++i : 1;
  }
  stiX::index_fn forward_search(std::string_view pattern) {
    return search(pattern, next_line);
  }

  size_t prev_line(size_t i, stiX::lines const& buffer) {
    return (i > 1) ? --i : buffer.last();
  }
  stiX::index_fn backward_search(std::string_view pattern) {
    return search(pattern, prev_line);
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

      has_line_numbers_when_forbidden();

      return command();
    }

    stiX::parsed_command command() {
      while(indicies.size() > 2)
        indicies.pop();

      if (is_error())
        return parse_error;

      return {from(), to(), code, filename};
    }

  private:
    void parse_line_numbers() {
      while (is_index_start()) {
        indicies.push(parse_line_number());

        if (is_separator())
          input.advance();
      }
    }

    stiX::index_fn parse_line_number() {
      auto lhs = parse_index();

      while (is_operator()) {
        auto op = input_pop();

        auto rhs = parse_number();

        if (op == MINUS)
          rhs *= -1;

        lhs = add_offset(lhs, rhs);
      }

      return lhs;
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
        case SLASH:
          return parse_forward_search();
        case BACKSLASH:
          return parse_backward_search();
      }

      return int_index(parse_number());
    }

    stiX::index_fn parse_forward_search() {
      return parse_search(SLASH, forward_search);
    }

    stiX::index_fn parse_backward_search() {
      return parse_search(BACKSLASH, backward_search);
    }

    stiX::index_fn parse_search(
      char delimiter,
      stiX::index_fn(make_search)(std::string_view)
    ) {
      input.advance();
      auto pattern = fetch_pattern(delimiter);
      input.advance();
      return make_search(pattern);
    }

    std::string fetch_pattern(char delimiter) {
      auto p = std::string{};
      while (!input.is_eol() && (*input != delimiter))
        p += input_pop();
      return p;
    }

    size_t parse_number() {
      auto n = fetch_digits();

      auto num = stiX::command::line_error;
      auto [_, ec] = std::from_chars(n.data(), n.data() + n.length(), num);

      if (ec != std::errc())
        failed();

      return num;
    }

    std::string fetch_digits() {
      auto n = std::string{};
      while (!input.is_eol() && std::isdigit(*input))
        n += input_pop();
      return n;
    }

    bool is_index_start() {
      auto c = *input;
      return c == DOT ||
             c == DOLLAR ||
             c == SLASH ||
             c == BACKSLASH ||
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

    void parse_command_code() {
      code = !input.is_eol() ? input_pop() : '\n';

      if (wants_filename(code))
        filename = parse_filename();

      if (!input.is_eol())
        failed();
    }

    bool wants_filename(char c) {
      auto file_codes = "efrw"s;
      return file_codes.find(c) != std::string::npos;
    }

    std::string parse_filename()
    {
      if (!std::isspace(input_pop()) || input.is_eol())
        failed();

      auto f = std::string{};
      while (!input.is_eol() && !std::isblank(*input))
        f += input_pop();
      return f;
    }

    bool has_line_numbers_when_forbidden() {
      auto forbidden_codes = "efq"s;
      auto forbidden = forbidden_codes.find(code) != std::string::npos;
      if (forbidden && !indicies.empty())
        failed();
    }

    char input_pop() {
      auto c = *input;
      input.advance();
      return c;
    }

    bool is_error() const {
      return has_failed || ::is_error(code);
    }

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
    std::string filename;
    bool has_failed = false;
  };

  size_t index_or_error(stiX::index_fn fn, stiX::lines const& buffer) {
    auto index = fn(buffer);
    return (index <= buffer.last()) ? index : stiX::command::line_error;
  }

  bool is_error(size_t from, size_t to, char code) {
    return is_error(from) ||
           is_error(to) ||
           (from > to) ||
           is_error(code);
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
  return { from, to, code, filename };
}

