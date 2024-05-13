
#include "commands.hpp"

void stiX::commands::operator()(
  std::istream& in,
  std::ostream& out,
  lines_modifier& buffer,
  std::string& filename) const {
  for(auto const& c : commands_)
    c(in, out, buffer, filename);
}
