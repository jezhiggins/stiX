
#include "replacement.hpp"
#include "../../lib/escapes.hpp"
#include <sstream>

static char next_char(
  std::string::const_iterator& c,
  std::string::const_iterator const& end
);
static char expand_escape_sequence(
  std::string::const_iterator& c
);

static auto const ditto_marker = std::string { 1, '\0' };

bool is_ditto(std::string const& str) {
  return str == ditto_marker;
}

std::string stiX::replacement::apply(std::string_view match) const {
  auto buffer = std::ostringstream { };
  apply(match, buffer);
  return buffer.str();
}

void stiX::replacement::apply(std::string_view match, std::ostream &out) const {
  for (auto const& r : replacements_) {
    if (is_ditto(r))
      out << match;
    else
      out << r;
  }
}

stiX::replacement stiX::prepare_replacement(std::string const& str) {
  auto replacements = std::vector<std::string> { };
  auto replacement = std::string { };

  auto push_replacement = [&replacements, &replacement](){
    if (!replacement.empty())
      replacements.emplace_back(replacement);
    replacement.clear();
  };
  auto push_ditto_marker = [&replacements]() {
    replacements.push_back(ditto_marker);
  };

  for (auto c = str.begin(); c != str.end(); ++c) {
    if (*c == '&') {
      push_replacement();
      push_ditto_marker();
      continue;
    }

    replacement += next_char(c, str.end());
  }

  push_replacement();

  return replacements;
}

char next_char(
  std::string::const_iterator& c,
  std::string::const_iterator const& end
) {
  auto is_escape = stiX::is_valid_escape_sequence(c, end);
  return is_escape ? expand_escape_sequence(c) : *c;
}

char expand_escape_sequence(
  std::string::const_iterator& c
) {
  auto candidate = *++c;
  return stiX::expand_escape(candidate);
} // expand_escape_sequence
