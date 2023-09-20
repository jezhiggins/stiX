#ifndef STICPP_COMMAND_ACTIONS_HPP
#define STICPP_COMMAND_ACTIONS_HPP

#include <iosfwd>

namespace stiX {
  class edit_buffer;
  class lines;

  void current_line_action(std::istream&, std::ostream& out, edit_buffer& buffer);
  void append_action(std::istream& in, size_t after, edit_buffer& buffer);
  void insert_action(std::istream& in, size_t before, edit_buffer& buffer);
  void delete_action(size_t from, size_t to, edit_buffer& buffer);
  void print_action(std::ostream& out, size_t from, size_t to, edit_buffer& buffer);

  void error_action(std::istream&, std::ostream&, edit_buffer&);
}

#endif