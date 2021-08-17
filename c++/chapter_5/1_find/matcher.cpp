#include "matcher.hpp"
#include "char_seq.hpp"
#include <utility>
#include "../../lib/escapes.hpp"
#include "../../lib/dash_range.hpp"

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

bool is_eol(const stiX::character_sequence& c) {
  return c.is_eol();
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

auto is_not_one_of_matcher(const std::string &targets) {
  return [targets](const stiX::character_sequence& c) {
    return targets.find(*c) == std::string::npos;
  };
}


stiX::match_fn_with_len make_character_class_matcher(stiX::character_sequence& characters);

stiX::match_fn_with_len make_matcher_fn(stiX::character_sequence& characters) {
  //if (characters.size() > 1)
  //  return stiX::match_fn_with_len(is_one_of_matcher(characters), true);

  char c = *characters;
  if (c == stiX::Escape && characters.available()) {
    characters.advance();
    char escaped = stiX::expand_escape(*characters);
    return stiX::match_fn_with_len(is_char_matcher(escaped), true);
  }
  if (c == '?')
    return stiX::match_fn_with_len(is_any_char, true);
  if (c == '%' && characters.is_bol())
    return stiX::match_fn_with_len(is_bol, false);
  if (c == '$' && !characters.available())
    return stiX::match_fn_with_len(is_eol, false);
  if (c == '[' )
    return make_character_class_matcher(characters);

  return stiX::match_fn_with_len(is_char_matcher(c), true);
}

stiX::match_fn_with_len make_character_class_matcher(stiX::character_sequence& characters) {
  auto subpattern = std::string { };

  characters.advance(); // step past '['
  for (char c = *characters; characters.available() && *characters != ']'; characters.advance(), c = *characters) {
    if (c == stiX::Escape && characters.available()) {
      characters.advance();
      c = stiX::expand_escape(*characters);
    }

    subpattern += c;
  }

  auto character_class = std::string { };
  auto subpat = stiX::character_sequence(subpattern);
  bool negated = false;
  if (*subpat == '^')
  {
    subpat.advance();
    negated = true;
  }
  for (char c = *subpat; !subpat.is_eol(); subpat.advance(), c = *subpat) {
    if (c == '-' && subpat.available() && !subpat.is_bol()) {
      char from = character_class.back() + 1;

      subpat.advance();
      char to = *subpat;

      if (stiX::is_dash_range(from, to))
        stiX::expand_dash_range(from, to, std::back_inserter(character_class));
      else
      {
        character_class += stiX::Dash;
        character_class += to;
      }

      continue;
    }

    character_class += c;
  }

  if (negated)
    return stiX::match_fn_with_len(is_not_one_of_matcher(character_class), true);
  return stiX::match_fn_with_len(is_one_of_matcher(character_class), true);
}

stiX::matcher stiX::make_matcher(stiX::character_sequence& characters) {
  return stiX::matcher(make_matcher_fn(characters));
}
