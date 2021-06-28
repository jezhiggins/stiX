#include "pattern_matcher.hpp"
#include "char_seq.hpp"

stiX::pattern_matcher::pattern_matcher(std::vector<stiX::matcher> m)
  : m_(std::move(m)){
}


bool match_all(const std::vector<stiX::matcher>& matchers, stiX::character_sequence seq) {
  for(auto& m : matchers) {
    if (!m.match(seq))
      return false;
    if (seq.is_eol())
      return false;
    if (m.consumes())
      seq.advance();
  }
  return true;
}

bool stiX::pattern_matcher::match(const std::string& line) const {
  for (auto seq = stiX::character_sequence(line); !seq.is_eol(); seq.advance()) {
    if (match_all(m_, seq))
      return true;
  }
  return false;
}

stiX::pattern_matcher stiX::compile_pattern(const std::string& pattern) {
  auto matches = std::vector<matcher> { };
  for (int i = 0; i != pattern.length(); ++i)
    matches.emplace_back(make_matcher(pattern.substr(i, 1)));

  return pattern_matcher(matches);
}
