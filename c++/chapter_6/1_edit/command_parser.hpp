#ifndef STICPP_COMMAND_PARSER_H
#define STICPP_COMMAND_PARSER_H

#include <string_view>

namespace stiX {
  struct command {
    size_t const from_index;
    size_t const to_index;
    char const code;

    command(size_t f, size_t t, char c) :
      from_index(f),
      to_index(t),
      code(c) {
    }
  };

  command parse_command(std::string_view input, size_t dot, size_t last);
}

#endif //STICPP_COMMAND_PARSER_H
