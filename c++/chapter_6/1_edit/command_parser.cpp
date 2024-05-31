#include "command_parser.hpp"
#include "edit_buffer.hpp"
#include <charconv>
#include <map>

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

  using stiX::code_match;

  bool is_error(char const c) { return c == stiX::command::code_error; }
  bool is_error(size_t const i) { return i == stiX::command::line_error; }

  stiX::line_expression int_index(size_t const index) {
    return [index](stiX::edit_buffer const&, size_t, std::string_view) -> std::tuple<size_t, std::string>
      { return { index, "" }; };
  }
  stiX::line_expression add_offset(stiX::line_expression const& ref_fn, size_t const index) {
    return [ref_fn, index](stiX::edit_buffer const& buffer, size_t const dot, std::string_view last_pat) -> std::tuple<size_t, std::string> {
      auto [i, p] = ref_fn(buffer, dot, last_pat);
      return { i + index, p };
    };
  }

  std::tuple<size_t, std::string> dot_index_fn(stiX::edit_buffer const&, size_t const dot, std::string_view) {
    return { dot, "" };
  }
  std::tuple<size_t, std::string> last_index_fn(stiX::edit_buffer const& buffer, size_t, std::string_view) {
    return { buffer.last(), "" };
  }
  std::tuple<size_t, std::string> line_error_fn(stiX::edit_buffer const&, size_t, std::string_view) {
    return { stiX::command::line_error, "" };
  }

  stiX::line_expression search(
    std::string const& search_pattern,
    size_t(next_index)(size_t, stiX::edit_buffer const&)
  ) {
    return [search_pattern, next_index](stiX::edit_buffer const& buffer, size_t const dot, std::string_view last_pat) ->
    std::tuple<size_t, std::string> {
      auto pattern = std::string { !search_pattern.empty() ? search_pattern : last_pat };
      if (pattern.empty())
        return { stiX::command::line_error, pattern };

      auto matcher = stiX::compile_pattern(pattern);
      size_t index = dot;
      do {
        index = next_index(index, buffer);

        if (matcher.match(buffer.line_at(index)))
          return { index, pattern };
      } while (index != dot);

      return { stiX::command::line_error, pattern };
    };
  }

  size_t next_line(size_t i, stiX::edit_buffer const& buffer) {
    return (i < buffer.last()) ? ++i : 1;
  }
  stiX::line_expression forward_search(std::string const& pattern) {
    return search(pattern, next_line);
  }

  size_t prev_line(size_t i, stiX::edit_buffer const& buffer) {
    return (i > 1) ? --i : buffer.last();
  }
  stiX::line_expression backward_search(std::string const& pattern) {
    return search(pattern, prev_line);
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
  stiX::command_extras with_search_replace(std::string p, std::string r, bool a) {
    return { .search_pattern = p, .replacement = r, .replace_all = a };
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
      if (code_match(code, "gwx")) {
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
      stiX::line_expression(make_search)(std::string const&)
    ) {
      input.advance();
      auto const pattern = fetch_pattern(delimiter);

      return make_search(pattern);
    }

    char fetch_delimiter() {
      auto const delimiter = *input;
      if (input.is_eol())
        failed();
      input.advance();

      return delimiter;
    }

    std::string fetch_pattern(char const delimiter) {
      if (input.is_eol())
        failed();

      auto p = std::string{};
      while (!input.is_eol() && (*input != delimiter))
        p += input_pop();

      if (input.is_eol())
        failed();

      input.advance();

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
      auto parameters = parse_command_parameters();

      if (and_then_print(code))
        parameters.and_print = parse_trailing_print();

      return parameters;
    }

    stiX::command_extras parse_command_parameters() {
      if (wants_filename(code))
        return parse_filename();

      if (wants_destination(code))
        return parse_destination();

      if (wants_search_replace(code))
        return parse_search_replace();

      if (wants_search_action(code))
        return parse_search_action();

      return no_extras();
    }

    static bool and_then_print(char const c) {
      return code_match(c, "d=ms");
    }

    static bool wants_filename(char const c) {
      return code_match(c, "efrw");
    }

    static bool wants_destination(char const c) {
      return code_match(c, "m");
    }

    static bool wants_search_replace(char const c) {
      return code_match(c, "s");
    }

    static bool wants_search_action(char const c) {
      return code_match(c, "gx");
    }

    bool parse_trailing_print() {
      if (input.is_eol())
        return false;

      if ('p' != input_pop())
        failed();

      return true;
    }

    stiX::command_extras parse_filename() {
      if (input.is_eol())
        return no_extras();

      if (!std::isspace(input_pop()))
        failed();

      strip_spaces();

      auto f = fetch_remainder();

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

      auto const delimiter = fetch_delimiter();

      auto const pattern = fetch_pattern(delimiter);

      auto const replacement = fetch_pattern(delimiter);

      auto replace_all = *input == 'g';
      if (replace_all) input.advance();

      return with_search_replace(pattern, replacement, replace_all);
    }

    stiX::command_extras parse_search_action() {
      if (input.is_eol())
        failed();

      auto const delimiter = fetch_delimiter();

      auto const pattern = fetch_pattern(delimiter);

      auto action = fetch_remainder();
      if (action.empty())
        failed();

      return with_search_replace(pattern, action, false);
    }

    std::string fetch_remainder() {
      auto s = std::string{};
      while (!input.is_eol())
        s += input_pop();

      while (std::isblank(s.back()))
        s.pop_back();

      return s;
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

  std::tuple<size_t, std::string> index_or_error(
    stiX::line_expression const& fn,
    stiX::edit_buffer const& buffer,
    size_t const dot,
    std::string_view previous_pattern
  ) {
    auto const [index, last_pattern] = fn(buffer, dot, previous_pattern);
    return {
      index <= buffer.last() ? index : stiX::command::line_error,
      last_pattern
    };
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

  using make_action_fn =
   std::function<stiX::action(
       size_t, size_t, size_t, stiX::command_extras const&)>;
  auto const command_map = std::map<char, make_action_fn> {
    { 'a', stiX::make_append_action },
    { 'c', stiX::make_change_action },
    { 'd', stiX::make_delete_action },
    { 'e', stiX::make_edit_action },
    { 'f', stiX::make_filename_action },
    { 'g', stiX::make_global_match_action },
    { 'i', stiX::make_insert_action },
    { 'm', stiX::make_move_action },
    { 'p', stiX::make_print_action },
    { '\n', stiX::make_print_next_line_action },
    { 'q', stiX::make_quit_action },
    { 'r', stiX::make_read_file_action },
    { 's', stiX::make_substitute_action },
    { 'w', stiX::make_write_file_action },
    { 'x', stiX::make_global_mismatch_action },
    { '=', stiX::make_line_index_action }
  };
} // namespace

stiX::action command_for_code(
    char const code,
    size_t const from_index,
    size_t const to_index,
    size_t const destination,
    stiX::command_extras const& extras) {

    auto const fn = command_map.find(code);

    return fn != command_map.cend()
      ? fn->second(from_index, to_index, destination, extras)
      : stiX::command::error;
}

stiX::parsed_command stiX::parse_command(std::string_view const input) {
  auto parser = command_parser(input);

  return parser.parse();
}

namespace {
  std::tuple<size_t, size_t, size_t, std::string> eval_line_expressions(
    std::vector<stiX::line_expression_step> const& line_expressions,
    stiX::edit_buffer const& buffer)
  {
    auto dot = buffer.dot();
    auto last_pattern = std::string { buffer.pattern() };
    auto line_numbers = std::vector<size_t> { };

    for (auto const& [expr, separator] : line_expressions) {
      auto [index, pattern] = index_or_error(expr, buffer, dot, last_pattern);
      dot = (separator == stiX::expression_separator::update) ? index : dot;
      last_pattern = !pattern.empty() ? pattern : last_pattern;
      line_numbers.push_back(index);
    }

    if (line_numbers.size() > 2)
      line_numbers.erase(line_numbers.begin(), line_numbers.end()-2);

    return { line_numbers.front(), line_numbers.back(), dot, last_pattern };
  }
}

stiX::commands stiX::parsed_command::compile(stiX::edit_buffer const& buffer) const {
  auto const [
    from,
    to,
    updated_dot,
    last_pattern] =
      eval_line_expressions(line_expressions, buffer);

  if (is_error(from, to, code))
    return { command::error };

  auto destination = command::line_error;
  if (extras.destination_expression != nullptr) {
    auto const [ d, _ ] = index_or_error(extras.destination_expression, buffer, updated_dot, last_pattern);
    if (is_error(d) || are_overlapping(from, to, d))
      return { command::error };
    destination = d;
  }

  auto set_dot = updated_dot != buffer.dot()
    ? command::update_dot(updated_dot)
    : command::noop;
  auto set_pattern = command::update_pattern(last_pattern);

  auto command = command_for_code(
      code,
      from,
      to,
      destination,
      extras);

  auto and_then = extras.and_print
    ? command::and_print
    : command::noop;

  return { set_dot, set_pattern, command, and_then } ;
}

bool stiX::code_match(char const c, std::string_view const codes) {
  return codes.find(c) != std::string::npos;
}
