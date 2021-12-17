#include "change.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include "replacement.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();

using site_type = stiX::pattern_matcher::size_type;

static bool is_zero_width_match(stiX::match_location loc);
static bool at_end(site_type offset, std::string_view line);
static bool not_at_end(site_type offset, std::string_view line);

static void apply_change(
  stiX::pattern_matcher const& matcher,
  std::vector<std::string> const& replacement,
  std::string_view input,
  std::ostream& out
);

static void insert_replacement(
  std::vector<std::string> const& replacements,
  std::string_view match,
  std::ostream& out);

void stiX::change(
  std::istream& in,
  std::ostream& out,
  std::string const& pattern,
  std::string const& subst
) {
  auto matcher = stiX::compile_pattern(pattern);
  auto replacement = stiX::prepare_replacement(subst);

  while(in.peek() != eof) {
    auto input = stiX::getline(in);

    apply_change(matcher, replacement, input, out);
  }
}

class replacement_state {
public:
  replacement_state() :
    state_(0) {
  }

  void next(bool at_end) { state_ = (state_ == 1) ? 2 : at_end; };
  bool completed() const { return state_ == 2; }

private:
  int state_;
};

void apply_change(
  stiX::pattern_matcher const& matcher,
  std::vector<std::string> const& replacement,
  std::string_view line,
  std::ostream &out
) {
  site_type offset = 0;
  site_type last_match = -1;

  for (auto state = replacement_state(); !state.completed(); state.next(at_end(offset, line))) {
    auto loc = matcher.find(line, offset);
    if (!loc.match)
      break;

    auto zero_width = is_zero_width_match(loc);

    if (last_match != loc.from || !zero_width) {
      auto up_to_match = line.substr(offset, loc.from - offset);
      out << up_to_match;
      insert_replacement(replacement, line.substr(loc.from, loc.to - loc.from), out);
    }

    offset = loc.to;
    last_match = loc.to;

    if (zero_width && not_at_end(offset, line)) {
      out << line[loc.from];
      offset = loc.from+1;
    }
  }

  if (not_at_end(offset, line))
    out << line.substr(offset);

  out << '\n';
}

void insert_replacement(
  std::vector<std::string> const& replacements,
  std::string_view match,
  std::ostream& out) {
  for (auto const& r : replacements) {
    if (stiX::is_ditto(r))
      out << match;
    else
      out << r;
  }
}

bool is_zero_width_match(stiX::match_location loc) {
  return loc.from == loc.to;
}

bool at_end(site_type offset, std::string_view line) {
  return offset == line.size();
}
bool not_at_end(site_type offset, std::string_view line) {
  return !at_end(offset, line);
}
