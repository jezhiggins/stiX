#include "format.hpp"

#include "../../lib/getline.hpp"

#include <sstream>
#include <string>

using namespace std::string_literals;

namespace {
  auto constinit eof = std::char_traits<char>::eof();
  auto constinit null = std::string_view { };
  auto constinit space = std::string_view { " " };
}

void stiX::formatter::operator()(std::istream &in, std::ostream &out) {
  auto separator = null;
  while (in.peek() != eof) {
    auto s = getline(in);

    out << separator << s;

    separator = space;
  }

  out << '\n';
}
