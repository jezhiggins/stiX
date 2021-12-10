#include "change.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();

static bool is_zero_width_match(stiX::match_location loc);
static bool not_at_end(int offset, std::string_view line);

static void apply_change(
  stiX::pattern_matcher const& matcher,
  std::string const& replacement,
  std::string_view input,
  std::ostream& out
);

void stiX::change(
  std::istream& in,
  std::ostream& out,
  std::string const& pattern,
  std::string const& replacement
) {
  auto matcher = stiX::compile_pattern(pattern);

  while(in.peek() != eof) {
    auto input = stiX::getline(in);

    apply_change(matcher, replacement, input, out);
  }
}

void apply_change(
  stiX::pattern_matcher const& matcher,
  std::string const& replacement,
  std::string_view line,
  std::ostream &out
) {
  auto offset = 0;
  auto last_match = -1;
  while(not_at_end(offset, line)) {
    auto loc = matcher.find(line, offset);
    if (!loc.match)
      break;

    auto prefix = line.substr(offset, loc.from - offset);
    if (last_match != loc.from)
      out << prefix << replacement;

    offset = loc.to;
    last_match = loc.to;

    if (is_zero_width_match(loc) && not_at_end(offset, line)) {
      out << line[loc.from];
      offset = loc.from+1;
    }
  }

  if (not_at_end(offset, line))
    out << line.substr(offset);
  else if (!matcher.is_terminal_only() && matcher.find(line, line.size()).match)
    out << replacement;

  out << '\n';
}

bool is_zero_width_match(stiX::match_location loc) {
  return loc.from == loc.to;
}

bool not_at_end(int offset, std::string_view line) {
  return offset != line.size();
}
