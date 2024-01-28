#include "command.hpp"

#include <limits>

#include "edit_buffer.hpp"

size_t const stiX::command::line_error = std::numeric_limits<size_t>::max();
char const stiX::command::code_error = '?';

void stiX::command::operator()(
  std::istream& in,
  std::ostream& out,
  edit_buffer& buffer,
  std::string& filename) const {
  if (dot != line_error)
    buffer.set_dot(dot);

  action(in, out, buffer, filename);
}

stiX::command const stiX::command::error = { };

