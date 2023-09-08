#include "command.hpp"
#include "lines.hpp"

size_t const stiX::command::line_error = -1;
char const stiX::command::code_error = '?';

void stiX::command::operator()(
  std::istream& in,
  std::ostream& out,
  edit_buffer& buffer) const {
  action(in, out, buffer);
}

stiX::command const stiX::command::error = { };

bool stiX::operator==(command const& lhs, command const& rhs) {
  return lhs.from_index == rhs.from_index &&
         lhs.to_index == rhs.to_index &&
         lhs.dot == rhs.dot &&
         lhs.code == rhs.code &&
         lhs.filename == rhs.filename;
}

bool stiX::operator!=(command const& lhs, command const& rhs) {
  return !(lhs == rhs);
}

