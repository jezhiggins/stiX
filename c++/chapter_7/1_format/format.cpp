#include "format.hpp"

#include "../../lib/getline.hpp"

#include <sstream>
#include <string>

namespace {
  auto constinit eof = std::char_traits<char>::eof();
}

void stiX::formatter::operator()(std::istream &in, std::ostream &out) {
  while (in.peek() != eof) {
    auto s = getline(in);
    out << s << '\n';
  }
}
