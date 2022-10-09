#ifndef STICPP_COMMAND_PARSER_H
#define STICPP_COMMAND_PARSER_H

#include <string_view>

namespace stiX {
  struct command {
    size_t const from_index = line_error;
    size_t const to_index = line_error;
    char const code = code_error;

    static size_t const line_error;
    static size_t const code_error;

    static command const error;
  };

  bool operator==(command const& lhs, command const& rhs);

  command parse_command(std::string_view input, size_t dot, size_t last);
}

#endif //STICPP_COMMAND_PARSER_H
