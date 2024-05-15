#include "command.hpp"

#include <ostream>

#include "edit_buffer.hpp"

void stiX::command::error(std::istream&, std::ostream& out, edit_buffer&, std::string&) {
  out << "?\n";
}
void stiX::command::noop(std::istream&, std::ostream&, edit_buffer&, std::string&) {
}
void stiX::command::and_print(std::istream&, std::ostream& out, edit_buffer& buffer, std::string&) {
  out << buffer.line_at(buffer.dot()) << '\n';
}
stiX::action stiX::command::update_dot(size_t const dot) {
  return [dot](std::istream&, std::ostream&, edit_buffer& buffer, std::string&) {
    buffer.set_dot(dot);
  };
}

