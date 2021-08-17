#include "pattern_matcher.hpp"
#include "char_seq.hpp"

stiX::match_stage::match_stage(matcher t)
  : test(std::move(t)), count(match_count::one) {
}

stiX::pattern_matcher::pattern_matcher(match_stages m)
  : m_(std::move(m)){
}

using match_stages_iter = stiX::match_stages::const_iterator;

static bool match_one(const stiX::matcher& matcher, stiX::character_sequence& seq);
static bool match_with_closure(match_stages_iter mbegin, const match_stages_iter& mend, stiX::character_sequence seq);
static bool match_all(match_stages_iter mbegin, const match_stages_iter& mend, stiX::character_sequence seq);
static bool match_all(const stiX::match_stages& matchers, stiX::character_sequence& seq);

bool match_one(const stiX::matcher& matcher, stiX::character_sequence& seq) {
  if (!matcher.match(seq))
    return false;
  if (matcher.consumes())
    seq.advance();
  return true;
}

bool match_with_closure(match_stages_iter mbegin, const match_stages_iter& mend, stiX::character_sequence seq) {
  seq.checkpoint();
  while(match_one(mbegin->test, seq));
  ++mbegin;
  do {
    if (match_all(mbegin, mend, seq))
      return true;
  } while(seq.rewind());

  return false;
}

bool match_all(match_stages_iter mbegin, const match_stages_iter& mend, stiX::character_sequence seq) {
  for(auto m = mbegin; m != mend; ++m) {
    switch (m->count) {
      case stiX::match_count::one:
        if (!match_one(m->test, seq))
          return false;
        break;
      case stiX::match_count::zero_or_more:
        return match_with_closure(m, mend, seq);
    }
  }
  return true;
}

bool match_all(const stiX::match_stages& matchers, stiX::character_sequence& seq) {
  return match_all(matchers.cbegin(), matchers.cend(), seq);
}

bool stiX::pattern_matcher::match(const std::string& line) const {
  bool once = true; // need to try at least once, because even zero length input might match
  for (auto seq = stiX::character_sequence(line); !seq.is_eol() || once; seq.advance(), once = false) {
    if (match_all(m_, seq))
      return true;
  }
  return false;
}

stiX::pattern_matcher stiX::compile_pattern(const std::string& pattern) {
  auto matches = match_stages { };

  for(auto seq = stiX::character_sequence(pattern); !seq.is_eol(); seq.advance()) {
    if (*seq == '*' && !matches.empty()) {
      matches.back().count = match_count::zero_or_more;
      continue;
    }

    matches.emplace_back(make_matcher(seq));
  }

  return pattern_matcher(matches);
}
