#ifndef STICPP_COMMAND_HPP
#define STICPP_COMMAND_HPP

#include <string>

namespace stiX {
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
}

#endif //STICPP_COMMAND_HPP
