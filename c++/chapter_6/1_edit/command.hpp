#ifndef STICPP_COMMAND_HPP
#define STICPP_COMMAND_HPP

#include <string>
#include "command_actions.hpp"

namespace stiX {
  class lines_modifier;

  struct command {
    action_fn const action = error_action;

    void operator()(
      std::istream& in,
      std::ostream& out,
      lines_modifier& buffer,
      std::string& filename) const;

    static size_t const line_error;
    static char const code_error;

    static command const error;
    static command const noop;
    static command const and_print;
    static command const update_dot(size_t dot);
  };
}

#endif //STICPP_COMMAND_HPP
