#include "command_parser.hpp"
#include "lines.hpp"
#include <charconv>
#include "../../lib/regex/pattern_matcher.hpp"
#include "../../lib/regex/char_seq.hpp"

using namespace std::string_view_literals;

namespace {
  constexpr auto DOT = '.';
  constexpr auto DOLLAR = '$';
  constexpr auto MINUS = '-';
  constexpr auto PLUS = '+';
  constexpr auto SLASH = '/';
  constexpr auto BACKSLASH = '\\';
  constexpr auto COMMA = ',';
  constexpr auto SEMI_COLON = ';';

  bool is_error(char const c) { return c == stiX::command::code_error; }
  bool is_error(size_t const i) { return i == stiX::command::line_error; }

  stiX::line_expression int_index(size_t const index) {
    return [index](stiX::lines const&, size_t) { return index; };
  }
  stiX::line_expression add_offset(stiX::line_expression const& ref_fn, size_t const index) {
    return [ref_fn, index](stiX::lines const& buffer, size_t const dot) {
      return ref_fn(buffer, dot) + index;
    };
  }

  size_t dot_index_fn(stiX::lines const&, size_t const dot) {
    return dot;
  }
  size_t last_index_fn(stiX::lines const& buffer, size_t) {
    return buffer.last();
  }

  stiX::line_expression search(
    std::string_view const pattern,
    size_t(next_index)(size_t, stiX::lines const&)
  ) {
    auto matcher = stiX::compile_pattern(pattern);
    return [matcher, next_index](stiX::lines const& buffer, size_t const dot) {
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
  stiX::line_expression forward_search(std::string_view const pattern) {
    return search(pattern, next_line);
  }

  size_t prev_line(size_t i, stiX::lines const& buffer) {
    return (i > 1) ? --i : buffer.last();
  }
  stiX::line_expression backward_search(std::string_view const pattern) {
    return search(pattern, prev_line);
  }

  size_t line_error_fn(stiX::lines const&, size_t) {
    return stiX::command::line_error;
  }

  stiX::parsed_command parse_error() {
    return {
      { { line_error_fn }, { line_error_fn } },
      stiX::command::code_error
    };
  }

  stiX::command_extras no_extras() { return { }; }
  stiX::command_extras with_filename(std::string f) { return { .filename = f }; }
  stiX::command_extras with_destination(stiX::line_expression d) {
    return { .destination_expression = d };
  }
  stiX::command_extras with_search_replace(std::string p, std::string r) {
    return { .search_pattern = p, .replacement = r };
  }

  class command_parser {
  public:
    explicit command_parser(std::string_view const i) :
      input(i) {
    }

    stiX::parsed_command parse() {
      parse_line_numbers();

      parse_command_code();

      has_line_numbers_when_forbidden();

      line_number_defaults();

      return command();
    }

    stiX::parsed_command command() const {
      if (is_error())
        return parse_error();

      return { indicies, code, extras };
    }

    void add_default_indicies() {
      if (code == 'w') {
        indicies.emplace_back(int_index(1), stiX::expression_separator::unchanged);
        indicies.emplace_back(last_index_fn, stiX::expression_separator::unchanged);
      }
      else if (code == '\n') {
        indicies.emplace_back(dot_index_fn, stiX::expression_separator::unchanged);
        indicies.emplace_back(add_offset(dot_index_fn, 1), stiX::expression_separator::update);
      }
      else
        indicies.emplace_back(dot_index_fn, stiX::expression_separator::unchanged);
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
        auto const op = input_pop();

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
        default:
          return int_index(parse_number());
      }
    }

    stiX::line_expression parse_forward_search() {
      return parse_search(SLASH, forward_search);
    }

    stiX::line_expression parse_backward_search() {
      return parse_search(BACKSLASH, backward_search);
    }

    stiX::line_expression parse_search(
      char const delimiter,
      stiX::line_expression(make_search)(std::string_view)
    ) {
      input.advance();
      auto const pattern = fetch_pattern(delimiter);
      input.advance();
      return make_search(pattern);
    }

    std::string fetch_pattern(char const delimiter) {
      if (input.is_eol())
        failed();

      auto p = std::string{};
      while (!input.is_eol() && (*input != delimiter))
        p += input_pop();

      if (input.is_eol())
        failed();

      return p;
    }

    size_t parse_number() {
      auto const n = fetch_digits();

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

    bool is_index_start() const {
      auto const c = *input;
      return c == DOT ||
             c == DOLLAR ||
             c == SLASH ||
             c == BACKSLASH ||
             is_operator() ||
             std::isdigit(c);
    }

    bool is_separator() const {
      auto const c = *input;
      return c == COMMA || c == SEMI_COLON;
    }

    stiX::expression_separator is_semi_colon() const {
      return static_cast<stiX::expression_separator>(*input == SEMI_COLON);
    }

    bool is_operator() const {
      auto c = *input;
      return c == PLUS || c == MINUS;
    }

    void parse_command_code() {
      code = !input.is_eol() ? input_pop() : '\n';

      extras = parse_extras();

      if (!input.is_eol())
        failed();
    }

    stiX::command_extras parse_extras() {
      if (wants_filename(code))
        return parse_filename();

      if (wants_destination(code))
        return parse_destination();

      if (wants_search_replace(code))
        return parse_search_replace();

      return no_extras();
    }

    static bool wants_filename(char const c) {
      return code_match(c, "efrw"sv);
    }

    static bool wants_destination(char const c) {
      return code_match(c, "m"sv);
    }

    static bool wants_search_replace(char const c) {
      return code_match(c, "s"sv);
    }

    static bool code_match(char const c, std::string_view const codes) {
      return codes.find(c) != std::string::npos;
    }

    stiX::command_extras parse_filename() {
      if (input.is_eol())
        return no_extras();

      if (!std::isspace(input_pop()))
        failed();

      strip_spaces();

      auto f = std::string{};
      while (!input.is_eol() && !std::isblank(*input))
        f += input_pop();

      return with_filename(f);
    }

    void strip_spaces() {
      while(std::isspace(*input))
        input_pop();
    }
    stiX::command_extras parse_destination() {
      strip_spaces();
      if (input.is_eol())
        failed();

      return with_destination(
        parse_line_number()
      );
    }

    stiX::command_extras parse_search_replace() {
      if (input.is_eol())
        failed();

      auto const delimiter = *input;
      if (input.is_eol())
        failed();
      input.advance();

      auto const pattern = fetch_pattern(delimiter);
      input.advance();

      auto const replacement = fetch_pattern(delimiter);
      input.advance();

      return with_search_replace(pattern, replacement);
    }

    void has_line_numbers_when_forbidden() {
      auto const forbidden = code_match(code, "efq"sv);
      if (forbidden && !indicies.empty())
        failed();
    }

    void line_number_defaults() {
      if (indicies.empty())
        add_default_indicies();

      if (indicies.size() < 2)
        indicies.emplace_back(indicies.front());
    }

    char input_pop() {
      auto const c = *input;
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
    stiX::command_extras extras;
    bool has_failed = false;
  };

  size_t index_or_error(stiX::line_expression const& fn, stiX::lines const& buffer, size_t const dot) {
    auto const index = fn(buffer, dot);
    return index <= buffer.last() ? index : stiX::command::line_error;
  }

  bool is_error(size_t const from, size_t const to, char const code) {
    return is_error(from) ||
           is_error(to) ||
           (from > to) ||
           is_error(code);
  }

  bool are_overlapping(size_t const from, size_t const to, size_t const destination) {
    return from <= destination && destination <= to;
  }
} // namespace

stiX::action_fn command_for_code(
    char const code,
    size_t const from_index,
    size_t const to_index,
    size_t const destination,
    std::string const& new_filename,
    std::string const& pattern,
    std::string const& replacement) {
  switch (code) {
    case 'a':
      return stiX::make_append_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'c':
      return stiX::make_change_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'd':
      return stiX::make_delete_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'e':
      return stiX::make_edit_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'f':
      return stiX::make_filename_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'i':
      return stiX::make_insert_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'm':
      return stiX::make_move_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'p':
      return stiX::make_print_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case '\n':
      return stiX::make_print_next_line_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'q':
      return stiX::make_quit_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'r':
      return stiX::make_read_file_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 's':
      return stiX::make_substitute_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case 'w':
      return stiX::make_write_file_action(from_index, to_index, destination, new_filename, pattern, replacement);
    case '=':
      return stiX::make_line_index_action(from_index, to_index, destination, new_filename, pattern, replacement);
    default:
      return stiX::error_action;
  }
}

stiX::parsed_command stiX::parse_command(std::string_view const input) {
  auto parser = command_parser(input);

  return parser.parse();
}

namespace {
  std::tuple<size_t, size_t, size_t> eval_line_expressions(
    std::vector<stiX::line_expression_step> const& line_expressions,
    stiX::lines const& buffer)
  {
    auto dot = buffer.dot();
    auto line_numbers = std::vector<size_t> { };

    for (auto const& [expr, separator] : line_expressions) {
      auto index = index_or_error(expr, buffer, dot);
      dot = (separator == stiX::expression_separator::update) ? index : dot;
      line_numbers.push_back(index);
    }

    if (line_numbers.size() > 2)
      line_numbers.erase(line_numbers.begin(), line_numbers.end()-2);

    return { line_numbers.front(), line_numbers.back(), dot };
  }
}

stiX::command stiX::parsed_command::compile(stiX::lines const& buffer) const {
  auto const [from, to, updated_dot] =
    eval_line_expressions(line_expressions, buffer);

  if (is_error(from, to, code))
    return command::error;

  auto destination = command::line_error;
  if (extras.destination_expression != nullptr) {
    destination = index_or_error(extras.destination_expression, buffer, updated_dot);
    if (is_error(destination) || are_overlapping(from, to,destination))
      return command::error;
  }

  return {
    from,
    to,
    updated_dot,
    code,
    command_for_code(
      code,
      from,
      to,
      destination,
      extras.filename,
      extras.search_pattern,
      extras.replacement)
  };
}

