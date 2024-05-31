
#include "commands.hpp"

void stiX::commands::operator()(
  std::istream& in,
  std::ostream& out,
  edit_buffer& buffer) const {
  for(auto const& c : commands_)
    c(in, out, buffer);
}

bool stiX::commands::is_error() const {
  return commands_.size() == 1;
}
