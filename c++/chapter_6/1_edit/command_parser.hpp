#ifndef STICPP_COMMAND_PARSER_H
#define STICPP_COMMAND_PARSER_H

#include <functional>
#include <vector>
#include "command.hpp"

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

  struct parsed_command {
    command compile(lines const& buffer) const;

    std::vector<line_expression_step> const line_expressions;
    char const code;
    std::string filename;
  };

  parsed_command parse_command(std::string_view input);
}

#endif //STICPP_COMMAND_PARSER_H
