#include "matcher.hpp"
#include "char_seq.hpp"
#include <utility>

stiX::matcher::matcher(match_fn_with_len match_fn)
  : fn_(std::move(match_fn.first)),
    consume_(match_fn.second) {
}

bool stiX::matcher::match(const stiX::character_sequence& candidate) const {
  return fn_(candidate);
}

bool stiX::matcher::consumes() const {
  return consume_;
}

bool is_any_char(const stiX::character_sequence& c) {
  return *c != '\n';
}

bool is_bol(const stiX::character_sequence& c) {
  return c.is_bol();
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

stiX::match_fn_with_len make_matcher_fn(const stiX::character_sequence& characters) {
  //if (characters.size() > 1)
  //  return stiX::match_fn_with_len(is_one_of_matcher(characters), true);

  char c = *characters;
  if (c == '?')
    return stiX::match_fn_with_len(is_any_char, true);
  if (c == '%' && characters.is_bol())
    return stiX::match_fn_with_len(is_bol, false);

  return stiX::match_fn_with_len(is_char_matcher(c), true);
}

stiX::matcher stiX::make_matcher(const stiX::character_sequence& characters) {
  return stiX::matcher(make_matcher_fn(characters));
}
