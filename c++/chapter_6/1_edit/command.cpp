#include "command.hpp"

#include <ostream>

#include "edit_buffer.hpp"

void stiX::command::error(std::istream&, std::ostream& out, edit_buffer&) {
  error_action(out);
}
void stiX::command::noop(std::istream&, std::ostream&, edit_buffer&) {
}
void stiX::command::and_print(std::istream&, std::ostream& out, edit_buffer& buffer) {
  out << buffer.line_at(buffer.dot()) << '\n';
}
stiX::action stiX::command::update_dot(size_t const dot) {
  return [dot](std::istream&, std::ostream&, edit_buffer& buffer) {
    buffer.set_dot(dot);
  };
}

