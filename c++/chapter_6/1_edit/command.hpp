#ifndef STICPP_COMMAND_HPP
#define STICPP_COMMAND_HPP

#include <string>
#include <functional>
#include "command_actions.hpp"

namespace stiX {
  class lines;
  struct command {
    using action_fn = std::function<void(std::istream&, std::ostream&, lines&)>;

    size_t const from_index = line_error;
    size_t const to_index = line_error;
    size_t const dot = line_error;
    char const code = code_error;
    std::string filename;
    action_fn const action = error_action;

    void operator()(
      std::istream& in,
      std::ostream& out,
      lines& buffer) const;

    static size_t const line_error;
    static char const code_error;

    static command const error;
  };

  bool operator==(command const& lhs, command const& rhs);
  bool operator!=(command const& lhs, command const& rhs);
}

#endif //STICPP_COMMAND_HPP
