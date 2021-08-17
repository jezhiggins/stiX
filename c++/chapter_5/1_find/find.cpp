#include "find.hpp"
#include "../../lib/getline.hpp"
#include "pattern_matcher.hpp"

#include <iostream>

void stiX::find(std::istream& in, std::ostream& out, std::string const& pattern) {
  auto matcher = stiX::compile_pattern(pattern);

  while(in) {
    auto line = stiX::getline(in);

    if (matcher.match(line))
      out << line << '\n';
  }
}
