
#include "commands.hpp"

void stiX::commands::operator()(
  std::istream& in,
  std::ostream& out,
  edit_buffer& buffer,
  std::string& filename) const {
  for(auto const& c : commands_)
    c(in, out, buffer, filename);
}

bool stiX::commands::is_error() const {
  action const e = commands_[0];
  return (e.target<void(
    std::istream&,
    std::ostream&,
    edit_buffer&,
    std::string&
  )>() == command::error);
}
