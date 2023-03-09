#ifndef STICPP_COMMAND_PARSER_H
#define STICPP_COMMAND_PARSER_H

#include <string_view>
#include <functional>

namespace stiX {
  class lines;
  using index_fn = std::function<size_t(lines const&)>;

  struct command {
    size_t const from_index = line_error;
    size_t const to_index = line_error;
    char const code = code_error;

    static size_t const line_error;
    static char const code_error;

    static command const error;
  };

  bool operator==(command const& lhs, command const& rhs);

  struct parsed_command {
    command compile(lines const& buffer) const;

    index_fn const from_index;
    index_fn const to_index;
    char const code;
  };

  parsed_command parse_command(std::string_view input);
}

#endif //STICPP_COMMAND_PARSER_H
