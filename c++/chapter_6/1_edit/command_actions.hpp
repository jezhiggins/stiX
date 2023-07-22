#ifndef STICPP_COMMAND_ACTIONS_HPP
#define STICPP_COMMAND_ACTIONS_HPP

#include <iosfwd>

namespace stiX {
  class edit_buffer;

  void do_current_line(std::ostream& out, edit_buffer& buffer);
  void do_append(std::istream& in, size_t after, edit_buffer& buffer);
  void do_insert(std::istream& in, size_t before, edit_buffer& buffer);
  void do_print(std::ostream& out, size_t from, size_t to, edit_buffer& buffer);
  void do_error(std::ostream& out);
}

#endif