#ifndef STICPP_COMMAND_PARSER_H
#define STICPP_COMMAND_PARSER_H

#include <functional>
#include <vector>
#include "commands.hpp"

namespace stiX {
  class edit_buffer;
  using line_expression = std::function<size_t(edit_buffer const&, size_t)>;

  enum class expression_separator : bool {
    unchanged = false,
    update = true
  };
  struct line_expression_step {
    line_expression expr;
    expression_separator separator;
  };
  struct command_extras {
    bool and_print;
    std::string filename;
    line_expression destination_expression;
    std::string search_pattern;
    std::string replacement;
    bool replace_all;
  };

  struct parsed_command {
    commands compile(edit_buffer const& buffer) const;

    std::vector<line_expression_step> const line_expressions;
    char const code;
    command_extras const extras;
  };

  parsed_command parse_command(std::string_view input);
}

#endif //STICPP_COMMAND_PARSER_H
