#include "command.hpp"

#include <limits>
#include <ostream>

#include "lines.hpp"

size_t const stiX::command::line_error = std::numeric_limits<size_t>::max();
char const stiX::command::code_error = '?';

void stiX::command::operator()(
  std::istream& in,
  std::ostream& out,
  lines_modifier& buffer,
  std::string& filename) const {
  action(in, out, buffer, filename);
}

stiX::command const stiX::command::error = { };
stiX::command const stiX::command::noop = {
  .action = [](std::istream&, std::ostream&, lines_modifier&, std::string&) { }
};
stiX::command const stiX::command::and_print = {
  .action = [](std::istream&, std::ostream& out, lines_modifier& buffer, std::string&) {
    out << buffer.line_at(buffer.dot()) << '\n';
  }
};
stiX::command const stiX::command::update_dot(size_t const dot) {
  auto update_dot_action = [dot](std::istream&, std::ostream&, lines_modifier& buffer, std::string&) {
    buffer.set_dot(dot);
  };
  return { .action = update_dot_action };
}

