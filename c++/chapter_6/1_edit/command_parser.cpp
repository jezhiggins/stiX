#include "command_parser.hpp"
#include "lines.hpp"
#include <charconv>
#include "../../lib/regex/pattern_matcher.hpp"
#include "../../lib/regex/char_seq.hpp"

using namespace std::string_literals;

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

  stiX::line_expression int_index(size_t index) {
    return [index](stiX::lines const&, size_t) { return index; };
  }
  stiX::line_expression add_offset(stiX::line_expression ref_fn, size_t index) {
    return [ref_fn, index](stiX::lines const& buffer, size_t dot) {
      return ref_fn(buffer, dot) + index;
    };
  }

  size_t dot_index_fn(stiX::lines const& buffer, size_t dot) {
    return dot;
  }
  size_t last_index_fn(stiX::lines const& buffer, size_t dot) {
    return buffer.last();
  }

  stiX::line_expression search(
    std::string_view pattern,
    size_t(next_index)(size_t, stiX::lines const&)
  ) {
    auto matcher = stiX::compile_pattern(pattern);
    return [matcher, next_index](stiX::lines const& buffer, size_t dot) {
      size_t index = dot;
      do {
        index = next_index(index, buffer);

        if (matcher.match(buffer.line_at(index)))
          return index;
      } while (index != dot);

      return stiX::command::line_error;
    };
  }

  size_t next_line(size_t i, stiX::lines const& buffer) {
    return (i < buffer.last()) ? ++i : 1;
  }
  stiX::line_expression forward_search(std::string_view pattern) {
    return search(pattern, next_line);
  }

  size_t prev_line(size_t i, stiX::lines const& buffer) {
    return (i > 1) ? --i : buffer.last();
  }
  stiX::line_expression backward_search(std::string_view pattern) {
    return search(pattern, prev_line);
  }

  size_t line_error_fn(stiX::lines const&, size_t) {
    return stiX::command::line_error;
  }

  stiX::parsed_command const parse_error = {
    { { line_error_fn }, { line_error_fn } },
    stiX::command::code_error
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
      if (is_error())
        return parse_error;

      if (indicies.empty())
        indicies.emplace_back(dot_index_fn, stiX::expression_separator::unchanged);

      if (indicies.size() < 2)
        indicies.emplace_back(indicies.front());

      return { indicies, code, filename };
    }

  private:
    void parse_line_numbers() {
      while (is_index_start()) {
        auto expression = parse_line_number();
        auto separator = is_semi_colon();

        indicies.emplace_back(expression, separator);

        if (is_separator())
          input.advance();
      }
    }

    stiX::line_expression parse_line_number() {
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

    stiX::line_expression parse_index() {
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

    stiX::line_expression parse_forward_search() {
      return parse_search(SLASH, forward_search);
    }

    stiX::line_expression parse_backward_search() {
      return parse_search(BACKSLASH, backward_search);
    }

    stiX::line_expression parse_search(
      char delimiter,
      stiX::line_expression(make_search)(std::string_view)
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

    stiX::expression_separator is_semi_colon() {
      return static_cast<stiX::expression_separator>(*input == SEMI_COLON);
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

    void has_line_numbers_when_forbidden() {
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

    stiX::character_sequence input;

    std::vector<stiX::line_expression_step> indicies;
    char code = stiX::command::code_error;
    std::string filename;
    bool has_failed = false;
  };

  size_t index_or_error(stiX::line_expression fn, stiX::lines const& buffer, size_t dot) {
    auto index = fn(buffer, dot);
    return (index <= buffer.last()) ? index : stiX::command::line_error;
  }

  bool is_error(size_t from, size_t to, char code) {
    return is_error(from) ||
           is_error(to) ||
           (from > to) ||
           is_error(code);
  }
} // namespace

stiX::command::action_fn command_for_code(char code, int to_index) {
  switch (code) {
    case 'a':
      return [to_index](std::istream& in, std::ostream&, stiX::edit_buffer& buffer) {
        stiX::append_action(in, to_index, buffer);
      };
    case 'i':
      return [to_index](std::istream& in, std::ostream&, stiX::edit_buffer& buffer) {
        stiX::insert_action(in, to_index, buffer);
      };
    case '=':
      return stiX::current_line_action;
  }
  return stiX::error_action;
}

stiX::parsed_command stiX::parse_command(std::string_view input) {
  auto parser = command_parser(input);

  return parser.parse();
}

stiX::command stiX::parsed_command::compile(stiX::lines const& buffer) const {
  auto dot = buffer.dot();
  auto line_numbers = std::vector<size_t> { };

  for (auto expression : line_expressions) {
    auto index = index_or_error(expression.expr, buffer, dot);
    dot = (expression.separator == expression_separator::update) ? index : dot;
    line_numbers.push_back(index);
  }

  if (line_numbers.size() > 2)
    line_numbers.erase(line_numbers.begin(), line_numbers.end()-2);

  auto from = line_numbers.front();
  auto to = line_numbers.back();

  if (is_error(from, to, code))
    return command::error;
  return { from, to, dot, code, filename, command_for_code(code, to) };
}

