#include "command.hpp"

#include <ostream>

#include "lines.hpp"

void stiX::command::error(std::istream&, std::ostream& out, lines_modifier&, std::string&) {
  out << "?\n";
}
void stiX::command::noop(std::istream&, std::ostream&, lines_modifier&, std::string&) {
}
void stiX::command::and_print(std::istream&, std::ostream& out, lines_modifier& buffer, std::string&) {
  out << buffer.line_at(buffer.dot()) << '\n';
}
stiX::action stiX::command::update_dot(size_t const dot) {
  return [dot](std::istream&, std::ostream&, lines_modifier& buffer, std::string&) {
    buffer.set_dot(dot);
  };
}

