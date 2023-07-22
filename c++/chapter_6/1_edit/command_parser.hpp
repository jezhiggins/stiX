#ifndef STICPP_COMMAND_PARSER_H
#define STICPP_COMMAND_PARSER_H

#include <string_view>
#include <functional>
#include <string>
#include <vector>

namespace stiX {
  class lines;
  using line_expression = std::function<size_t(lines const&, size_t)>;

  enum class expression_separator : bool {
    unchanged = false,
    update = true
  };
  struct line_expression_step {
    line_expression expr;
    expression_separator separator;
  };

  struct command {
    size_t const from_index = line_error;
    size_t const to_index = line_error;
    size_t const dot = line_error;
    char const code = code_error;
    std::string filename;

    static size_t const line_error;
    static char const code_error;

    static command const error;
  };

  bool operator==(command const& lhs, command const& rhs);
  bool operator!=(command const& lhs, command const& rhs);

  struct parsed_command {
    command compile(lines const& buffer) const;

    std::vector<line_expression_step> const line_expressions;
    char const code;
    std::string filename;
  };

  parsed_command parse_command(std::string_view input);
}

#endif //STICPP_COMMAND_PARSER_H
