#ifndef STICPP_COMMAND_ACTIONS_HPP
#define STICPP_COMMAND_ACTIONS_HPP

#include <iosfwd>
#include <string>
#include <functional>

namespace stiX {
  class lines_modifier;
  class lines;
  class parsed_command;

  using action =
    std::function<void(
      std::istream&,
      std::ostream&,
      lines_modifier&,
      std::string&
    )>;
  class command_extras;

  void line_index_action(std::ostream& out, size_t to);
  void append_action(std::istream& in, size_t after, lines_modifier& buffer);
  void insert_action(std::istream& in, size_t before, lines_modifier& buffer);
  void change_action(std::istream& in, size_t from, size_t to, lines_modifier& buffer);
  void move_action(size_t from, size_t to, size_t after, lines_modifier& buffer);
  void delete_action(size_t from, size_t to, lines_modifier& buffer);
  void print_action(std::ostream& out, size_t from, size_t to, lines_modifier& buffer);
  void filename_action(std::string_view filename, std::string& property, std::ostream& out);
  void write_to_file_action(size_t from, size_t to, std::string_view filename, std::string& property, lines_modifier& buffer);
  void read_from_file_action(size_t before, std::string_view filename, std::string& property, lines_modifier& buffer);
  void edit_file_action(std::string_view filename, std::string& property, lines_modifier& buffer);
  void substitute_action(size_t from, size_t to, std::string_view pattern, std::string_view replacement, bool replace_all, lines_modifier& buffer);
  void global_action(size_t from, size_t to, std::string_view pattern, stiX::parsed_command const& action,
                     std::istream& in, std::ostream& out, lines_modifier& buffer, std::string& filename);

  action make_append_action(size_t from_index,
                            size_t to_index,
                            size_t destination,
                            command_extras const& extras);
  action make_change_action(size_t from_index,
                            size_t to_index,
                            size_t destination,
                            command_extras const& extras);
  action make_delete_action(size_t from_index,
                            size_t to_index,
                            size_t destination,
                            command_extras const& extras);
  action make_edit_action(size_t from_index,
                          size_t to_index,
                          size_t destination,
                          command_extras const& extras);
  action make_filename_action(size_t from_index,
                              size_t to_index,
                              size_t destination,
                              command_extras const& extras);
  action make_global_action(size_t from_index,
                            size_t to_index,
                            size_t destination,
                            command_extras const& extras);
  action make_insert_action(size_t from_index,
                            size_t to_index,
                            size_t destination,
                            command_extras const& extras);
  action make_move_action(size_t from_index,
                          size_t to_index,
                          size_t destination,
                          command_extras const& extras);
  action make_print_action(size_t from_index,
                           size_t to_index,
                           size_t destination,
                           command_extras const& extras);
  action make_print_next_line_action(size_t from_index,
                                     size_t to_index,
                                     size_t destination,
                                     command_extras const& extras);
  action make_quit_action(size_t from_index,
                          size_t to_index,
                          size_t destination,
                          command_extras const& extras);
  action make_read_file_action(size_t from_index,
                               size_t to_index,
                               size_t destination,
                               command_extras const& extras);
  action make_substitute_action(size_t from_index,
                                size_t to_index,
                                size_t destination,
                                command_extras const& extras);
  action make_write_file_action(size_t from_index,
                                size_t to_index,
                                size_t destination,
                                command_extras const& extras);
  action make_line_index_action(size_t from_index,
                                size_t to_index,
                                size_t destination,
                                command_extras const& extras);
}

#endif