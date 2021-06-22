#include "matcher.hpp"

#include <utility>

stiX::matcher::matcher(match_fn fn)
  : fn_(fn) {
}

bool stiX::matcher::match(char candidate) const {
  return fn_(candidate);
}

auto is_char_matcher(char target) {
  return [target](char c) {
    return c == target;
  };
}

auto is_one_of_matcher(const std::string &targets) {
  return [targets](char c) {
    return targets.find(c) != std::string::npos;
  };
}

stiX::matcher stiX::make_matcher(const std::string& characters) {
  if (characters.size() > 1)
    return matcher(is_one_of_matcher(characters));

  return stiX::matcher(is_char_matcher(characters[0]));
}
