#include "change.hpp"
#include "../getline.hpp"
#include "pattern_matcher.hpp"
#include "replacement.hpp"

#include <iostream>

auto const eof = std::char_traits<char>::eof();

using size_type = stiX::pattern_matcher::size_type;

namespace {
  bool at_end(std::string_view line, size_type offset) {
    return offset == line.size();
  }
  bool not_at_end(std::string_view line, size_type offset) {
    return !at_end(line, offset);
  }

  class replacement_state {
  public:
    replacement_state() : state_(0) { }

    void next(bool at_end) { state_ = (state_ == 1) ? 2 : at_end; };
    bool completed() const { return state_ == 2; }

  private:
    int state_;
  };

  struct match_offsets {
    size_t offset;
    size_t last_match;

    match_offsets() : offset(0), last_match(-1) { }
    match_offsets(size_t o, size_t l) : offset(o), last_match(l) { }
  };

  bool at_end(std::string_view line, match_offsets offsets) {
    return at_end(line, offsets.offset) || at_end(line, offsets.last_match);
  }

  match_offsets apply_change_at_offset(
    stiX::pattern_matcher const& matcher,
    stiX::replacement const& replacer,
    match_offsets mo,
    std::string_view line,
    std::ostream& out
  ) {
    auto [ offset, last_match ] = mo;
    auto match = matcher.find(line, offset);

    if (!match.match)
      return { offset, line.size() };

    if (!match.zero_width || last_match != match.from) {
      auto up_to_match = line.substr(offset, match.from - offset);
      auto match_text = line.substr(match.from, match.length);

      out << up_to_match << replacer.apply(match_text);
    }

    offset = match.to;
    last_match = match.to;

    if (match.zero_width && not_at_end(line, offset)) {
      out << line[match.from];
      offset = match.from + 1;
    }

    return { offset, last_match };
  }
}

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

    out << '\n';
  }
}

void stiX::apply_change(
  pattern_matcher const& matcher,
  replacement const& replacer,
  std::string_view line,
  std::ostream& out
) {
  auto offsets = match_offsets { };

  for (auto state = replacement_state(); !state.completed(); state.next(at_end(line, offsets)))
    offsets = apply_change_at_offset(matcher, replacer, offsets, line, out);

  out << line.substr(offsets.offset);
}

void stiX::apply_change_once(
  pattern_matcher const& matcher,
  replacement const& replacer,
  std::string_view line,
  std::ostream& out
) {
  auto [offset, last_match] = apply_change_at_offset(matcher, replacer, match_offsets{} , line, out);
  out << line.substr(offset);
}
