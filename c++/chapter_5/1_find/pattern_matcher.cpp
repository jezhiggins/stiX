#include "pattern_matcher.hpp"

stiX::pattern_matcher::pattern_matcher(std::vector<stiX::matcher> m)
  : m_(std::move(m)){
}


bool match_all(const std::vector<stiX::matcher>& matchers, const std::string& line, int i) {
  for(auto& m : matchers) {
    if (!m.match(line[i]))
      return false;
    if (++i > line.length())
      return false;
  }
  return true;
}

bool stiX::pattern_matcher::match(const std::string& line) const {
  int i = 0;
  while (i != line.length()) {
    if (match_all(m_, line, i))
      return true;
    ++i;
  }
  return false;
}

stiX::pattern_matcher stiX::compile_pattern(const std::string& pattern) {
  auto matches = std::vector<matcher> { };
  for (int i = 0; i != pattern.length(); ++i)
    matches.emplace_back(make_matcher(pattern.substr(i, 1)));

  return pattern_matcher(matches);
}
