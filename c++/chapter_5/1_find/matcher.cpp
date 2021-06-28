#include "matcher.hpp"
#include "char_seq.hpp"
#include <utility>

stiX::matcher::matcher(match_fn fn)
  : fn_(std::move(fn)) {
}

bool stiX::matcher::match(const stiX::character_sequence& candidate) const {
  return fn_(candidate);
}

bool is_any_char(const stiX::character_sequence& c) {
  return *c != '\n';
}

auto is_char_matcher(char target) {
  return [target](const stiX::character_sequence& c) {
    return *c == target;
  };
}

auto is_one_of_matcher(const std::string &targets) {
  return [targets](const stiX::character_sequence& c) {
    return targets.find(*c) != std::string::npos;
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
