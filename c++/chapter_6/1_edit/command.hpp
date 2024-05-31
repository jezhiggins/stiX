#ifndef STICPP_COMMAND_HPP
#define STICPP_COMMAND_HPP

#include "command_actions.hpp"
#include <limits>
#include <limits>

namespace stiX {
  class edit_buffer;

  namespace command {
    constexpr size_t const line_error = std::numeric_limits<size_t>::max();
    constexpr char const code_error = '?';

    void error(std::istream&, std::ostream&, edit_buffer&);
    void noop(std::istream&, std::ostream&, edit_buffer&);
    void and_print(std::istream&, std::ostream&, edit_buffer&);
    action update_dot(size_t dot);
    action update_pattern(std::string const& last_pattern);
  };
}

#endif //STICPP_COMMAND_HPP
