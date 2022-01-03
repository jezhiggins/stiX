#include "change.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include "replacement.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();

using size_type = stiX::pattern_matcher::size_type;

static bool at_end(std::string_view line, size_type offset);
static bool not_at_end(std::string_view line, size_type offset);

static void apply_change(
  stiX::pattern_matcher const& matcher,
  stiX::replacement const& replacer,
  std::string_view input,
  std::ostream& out
);

void stiX::change(
  std::istream& in,
  std::ostream& out,
  std::string const& pattern,
  std::string const& subst
) {
  auto matcher = stiX::compile_pattern(pattern);
  auto replacer = stiX::prepare_replacement(subst);

  while(in.peek() != eof) {
    auto input = stiX::getline(in);

    apply_change(matcher, replacer, input, out);
  }
}

class replacement_state {
public:
  replacement_state() : state_(0) { }

  void next(bool at_end) { state_ = (state_ == 1) ? 2 : at_end; };
  bool completed() const { return state_ == 2; }

private:
  int state_;
};

void apply_change(
  stiX::pattern_matcher const& matcher,
  stiX::replacement const& replacer,
  std::string_view line,
  std::ostream &out
) {
  size_type offset = 0;
  size_type last_match = -1;

  for (auto state = replacement_state(); !state.completed(); state.next(at_end(line, offset))) {
    auto match = matcher.find(line, offset);
    if (!match.match)
      break;

    if (last_match != match.from || !match.zero_width) {
      auto up_to_match = line.substr(offset, match.from - offset);
      out << up_to_match;
      replacer.apply(line.substr(match.from, match.length), out);
    }

    offset = match.to;
    last_match = match.to;

    if (match.zero_width && not_at_end(line, offset)) {
      out << line[match.from];
      offset = match.from + 1;
    }
  }

  out << line.substr(offset) << '\n';
}

bool at_end(std::string_view line, size_type offset) {
  return offset == line.size();
}
bool not_at_end(std::string_view line, size_type offset) {
  return !at_end(line, offset);
}
