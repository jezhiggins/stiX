#ifndef STICPP_COMMAND_HPP
#define STICPP_COMMAND_HPP

#include <string>
#include "command_actions.hpp"

namespace stiX {
  class edit_buffer;

  struct command {
    size_t const from_index = line_error;
    size_t const to_index = line_error;
    size_t const dot = line_error;
    char const code = code_error;
    action_fn const action = error_action;

    void operator()(
      std::istream& in,
      std::ostream& out,
      edit_buffer& buffer,
      std::string& filename) const;

    static size_t const line_error;
    static char const code_error;

    static command const error;
    static command const noop;
    static command const and_print;
  };

  bool operator==(command const& lhs, command const& rhs);
  bool operator!=(command const& lhs, command const& rhs);
}

#endif //STICPP_COMMAND_HPP
