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
  return !c.is_eol() && (*c != '\n');
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
    return !c.is_eol() && (targets.find(*c) == std::string::npos);
  };
}

static stiX::match_fn_with_len
  make_character_class_matcher(stiX::character_sequence& characters);
static char escape_char(stiX::character_sequence& characters);

static char const match_any_char = '?';
static char const match_beginning_of_line = '%';
static char const match_end_of_line = '$';
static char const start_of_class = '[';
static char const end_of_class = ']';
static char const negate_class = '^';

stiX::match_fn_with_len make_matcher_fn(stiX::character_sequence& characters) {
  char c = *characters;

  if (c == match_any_char)
    return { is_any_char, true };

  if (c == match_beginning_of_line && characters.is_bol())
    return { is_bol, false };

  if (c == match_end_of_line && !characters.available())
    return { is_eol, false };

  if (c == start_of_class)
    return make_character_class_matcher(characters);

  if (c == stiX::Escape)
    c = escape_char(characters);

  return { is_char_matcher(c), true };
}

stiX::match_fn_with_len make_character_class_matcher(stiX::character_sequence& characters) {
  auto subpattern = std::string { };

  characters.advance(); // step past '['
  for (char c = *characters; characters.available() && *characters != end_of_class; characters.advance(), c = *characters) {
    if (c == stiX::Escape)
      c = escape_char(characters);
    subpattern += c;
  }

  auto character_class = std::string { };
  auto subpat = stiX::character_sequence(subpattern);
  bool negated = false;
  if (*subpat == negate_class)
  {
    subpat.advance();
    negated = true;
  }
  for (char c = *subpat; !subpat.is_eol(); subpat.advance(), c = *subpat) {
    if (c == stiX::Dash && subpat.available() && !subpat.is_bol()) {
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
    }
    else
      character_class += c;
  }

  if (negated)
    return { is_not_one_of_matcher(character_class), true };
  return { is_one_of_matcher(character_class), true };
}

char escape_char(stiX::character_sequence& characters)
{
  if (!characters.available())
    return *characters;

  characters.advance();
  return stiX::expand_escape(*characters);
}

stiX::matcher stiX::make_matcher(stiX::character_sequence& characters) {
  return stiX::matcher(make_matcher_fn(characters));
}
