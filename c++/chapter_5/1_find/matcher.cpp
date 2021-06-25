#include "matcher.hpp"

#include <utility>

stiX::matcher::matcher(match_fn fn)
  : fn_(std::move(fn)) {
}

bool stiX::matcher::match(char candidate) const {
  return fn_(candidate);
}

bool is_any_char(char target) {
  return target != '\n';
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

stiX::match_fn make_matcher_fn(const std::string& characters) {
  if (characters.size() > 1)
    return is_one_of_matcher(characters);

  char c = characters[0];
  if (c == '?')
    return is_any_char;

  return is_char_matcher(c);
}

stiX::matcher stiX::make_matcher(const std::string& characters) {
  return stiX::matcher(make_matcher_fn(characters));
}
