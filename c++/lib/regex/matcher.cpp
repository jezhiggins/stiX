#include "matcher.hpp"
#include "char_seq.hpp"
#include <utility>
#include "../escapes.hpp"
#include "../dash_range.hpp"

stiX::matcher::matcher(match_fn_with_consume match_fn)
  : fn_(std::move(match_fn.first)),
    consume_(match_fn.second) {
}

bool stiX::matcher::match(stiX::character_sequence const& candidate) const {
  return fn_(candidate);
}

bool stiX::matcher::consumes() const {
  return consume_;
}

bool is_any_char(stiX::character_sequence const& c) {
  return !c.is_eol() && (*c != '\n');
}

bool is_bol(stiX::character_sequence const& c) {
  return c.is_bol();
}

bool is_eol(stiX::character_sequence const& c) {
  return c.is_eol();
}

auto is_char_matcher(char target) {
  return [target](stiX::character_sequence const& c) {
    return *c == target;
  };
}

auto is_one_of_matcher(std::string const& targets) {
  return [targets](stiX::character_sequence const& c) {
    return targets.find(*c) != std::string::npos;
  };
}

auto is_not_one_of_matcher(std::string const& targets) {
  return [targets](stiX::character_sequence const& c) {
    return !c.is_eol() && (targets.find(*c) == std::string::npos);
  };
}

static stiX::match_fn_with_consume
  make_character_class_matcher(stiX::character_sequence& characters);
static std::string expand_dash(char from, char to);
static char escape_char(stiX::character_sequence& characters);

static char const match_any_char = '?';
static char const match_beginning_of_line = '%';
static char const match_end_of_line = '$';
static char const start_of_class = '[';
static char const end_of_class = ']';
static char const negate_class = '^';

stiX::match_fn_with_consume make_matcher_fn(stiX::character_sequence& characters) {
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

static std::string extract_character_class(stiX::character_sequence& characters) {
  auto subpattern = std::string { };

  characters.advance(); // step past '['
  for (char c = *characters; characters.available() && *characters != end_of_class; characters.advance(), c = *characters) {
    if (c == stiX::Escape)
      c = escape_char(characters);
    subpattern += c;
  }

  return subpattern;
}

static bool is_negated_match(stiX::character_sequence& subpat) {
  if (*subpat != negate_class)
    return false;

  subpat.advance();
  return true;
}

stiX::match_fn_with_consume make_character_class_matcher(stiX::character_sequence& characters) {
  auto subpattern = extract_character_class(characters);

  auto subpat = stiX::character_sequence(subpattern);
  auto negated = is_negated_match(subpat);

  auto character_class = std::string { };
  for (char c = *subpat; !subpat.is_eol(); subpat.advance(), c = *subpat) {
    if (c == stiX::Dash && subpat.available() && !subpat.is_bol()) {
      char from = character_class.back() + 1;
      subpat.advance();
      char to = *subpat;

      character_class += expand_dash(from, to);
    }
    else
      character_class += c;
  }

  if (negated)
    return { is_not_one_of_matcher(character_class), true };
  return { is_one_of_matcher(character_class), true };
}

std::string expand_dash(char from, char to) {
  auto expansion = std::string { };

  if (stiX::is_dash_range(from, to))
    stiX::expand_dash_range(from, to, std::back_inserter(expansion));
  else
  {
    expansion += stiX::Dash;
    expansion += to;
  }

  return expansion;
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
