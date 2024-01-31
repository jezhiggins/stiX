#ifndef STICPP_COMMAND_ACTIONS_HPP
#define STICPP_COMMAND_ACTIONS_HPP

#include <iosfwd>
#include <string>
#include <functional>

namespace stiX {
  class edit_buffer;
  class lines;

  using action_fn =
    std::function<void(
      std::istream&,
      std::ostream&,
      edit_buffer&,
      std::string&
    )>;

  void line_index_action(std::ostream& out, size_t to);
  void append_action(std::istream& in, size_t after, edit_buffer& buffer);
  void insert_action(std::istream& in, size_t before, edit_buffer& buffer);
  void change_action(std::istream& in, size_t from, size_t to, edit_buffer& buffer);
  void move_action(size_t from, size_t to, size_t after, edit_buffer& buffer);
  void delete_action(size_t from, size_t to, edit_buffer& buffer);
  void print_action(std::ostream& out, size_t from, size_t to, edit_buffer& buffer);
  void filename_action(std::string_view filename, std::string& property, std::ostream& out);
  void write_to_file_action(size_t from, size_t to, std::string_view filename, std::string& property, edit_buffer& buffer);
  void read_from_file_action(size_t before, std::string_view filename, std::string& property, edit_buffer& buffer);
  void edit_file_action(std::string_view filename, std::string& property, edit_buffer& buffer);
  void substitute_action(size_t from, size_t to, std::string_view pattern, std::string_view replace, edit_buffer& buffer);

  void error_action(std::istream&, std::ostream&, edit_buffer&, std::string&);

  action_fn make_append_action(size_t const from_index,
    size_t const to_index,
    size_t const destination,
    std::string const& new_filename,
    std::string const& pattern,
    std::string const& replacement);
  action_fn make_change_action(size_t const from_index,
    size_t const to_index,
    size_t const destination,
    std::string const& new_filename,
    std::string const& pattern,
    std::string const& replacement);
}

#endif