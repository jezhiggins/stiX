#include "change.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();

void stiX::change(
  std::istream& in,
  std::ostream& out,
  std::string const& pattern,
  std::string const& replacement
) {
  auto matcher = stiX::compile_pattern(pattern);

  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto remainder = std::string_view(line);

    while(true) {
      auto loc = matcher.find(remainder);
      if (!loc.match)
        break;

      auto prefix = remainder.substr(0, loc.from);
      remainder = remainder.substr(loc.to);

      out << prefix << replacement;
    }
    out << remainder << '\n';
  }
}
