#include "find.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();

void stiX::find(std::istream& in, std::ostream& out, std::string const& pattern) {
  auto matcher = stiX::compile_pattern(pattern);

  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (matcher.match(line))
      out << line << '\n';
  }
}
