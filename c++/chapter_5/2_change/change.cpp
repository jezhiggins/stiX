#include "change.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();

static bool is_zero_width_match(stiX::match_location loc) {
  return loc.from == loc.to;
}

void stiX::change(
  std::istream& in,
  std::ostream& out,
  std::string const& pattern,
  std::string const& replacement
) {
  auto matcher = stiX::compile_pattern(pattern);

  while(in.peek() != eof) {
    auto input = stiX::getline(in);
    auto line = std::string_view(input);

    auto offset = 0;
    while(offset != line.size()) {
      auto loc = matcher.find(line, offset);
      if (!loc.match)
        break;

      auto prefix = line.substr(offset, loc.from-offset);
      out << prefix << replacement;

      offset = loc.to;
      if (is_zero_width_match(loc) && offset != line.size()) {
        out << line[loc.from];
        offset = loc.from+1;
      }
    }

    if (offset != line.size())
      out << line.substr(offset);
    else if (!matcher.is_terminal_only() && matcher.find(line, line.size()).match)
      out << replacement;

    out << '\n';
  }
}
