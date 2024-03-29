#include "pattern_matcher.hpp"
#include "char_seq.hpp"

stiX::pattern::pattern(matcher t)
  : test(std::move(t)), count(match_count::one) {
}

stiX::pattern_matcher::pattern_matcher(patterns m)
  : m_(std::move(m)){
}

using match_stages_iter = stiX::patterns::const_iterator;

static bool match_one(stiX::matcher const& matcher, stiX::character_sequence& seq);
static stiX::match_location match_with_closure(match_stages_iter mbegin, match_stages_iter const& mend, stiX::character_sequence seq);
static stiX::match_location match_all(match_stages_iter mbegin, match_stages_iter const& mend, stiX::character_sequence seq);
static stiX::match_location match_all(stiX::patterns const& matchers, stiX::character_sequence& seq);
static stiX::match_location match_terminal_only(stiX::matcher const& matcher, stiX::character_sequence& seq);

static auto not_found = stiX::match_location { false, std::string_view::npos, std::string_view::npos };

bool match_one(stiX::matcher const& matcher, stiX::character_sequence& seq) {
  if (!matcher.match(seq))
    return false;
  if (matcher.consumes())
    seq.advance();
  return true;
}

stiX::match_location match_with_closure(match_stages_iter mbegin, match_stages_iter const& mend, stiX::character_sequence seq) {
  seq.checkpoint();
  while(match_one(mbegin->test, seq));
  ++mbegin;
  do {
    auto m = match_all(mbegin, mend, seq);
    if (m.match)
      return m;
  } while(seq.rewind());

  return not_found;
}

stiX::match_location match_all(match_stages_iter mbegin, match_stages_iter const& mend, stiX::character_sequence seq) {
  for(auto m = mbegin; m != mend; ++m) {
    switch (m->count) {
      case stiX::match_count::one:
        if (!match_one(m->test, seq))
          return not_found;
        break;
      case stiX::match_count::zero_or_more:
        return match_with_closure(m, mend, seq);
    }
  }

  return { true, seq.from(), seq.to() };
}

stiX::match_location match_all(stiX::patterns const& matchers, stiX::character_sequence& seq) {
  seq.mark_from();
  return match_all(matchers.cbegin(), matchers.cend(), seq);
}

stiX::match_location match_terminal_only(stiX::matcher const& matcher, stiX::character_sequence& seq) {
  if (match_one(matcher, seq))
    return { true, seq.from(), seq.from() };

  seq.skip_to_end();
  if (match_one(matcher, seq))
    return { true, seq.to(), seq.to() };

  return not_found;
}

stiX::match_location stiX::pattern_matcher::find(std::string_view line, size_type offset) const {
  auto seq = stiX::character_sequence(line, offset);

  if (is_terminal_only())
    return match_terminal_only(m_.front().test, seq);

  bool once = true; // need to try at least once, because even zero length input might match
  for (; !seq.is_eol() || once; seq.advance(), once = false) {
    auto match = match_all(m_, seq);
    if (match.match) {
      return match;
    }
  }
  return not_found;
}

bool stiX::pattern_matcher::match(std::string_view line, size_type offset) const {
  return find(line, offset).match;
}

bool stiX::pattern_matcher::is_terminal_only() const {
  return (m_.size() == 1) && (!m_.front().test.consumes());
}

static char const kleene_star = '*';

stiX::pattern_matcher stiX::compile_pattern(std::string_view pattern) {
  auto matches = patterns { };

  for(auto seq = stiX::character_sequence(pattern); !seq.is_eol(); seq.advance()) {
    if (*seq == kleene_star && !matches.empty()) {
      matches.back().count = match_count::zero_or_more;
      continue;
    }

    matches.emplace_back(make_matcher(seq));
  }

  return pattern_matcher(matches);
}
