
#include "replacement.hpp"
#include "../../lib/escapes.hpp"

static bool is_valid_escape_sequence(
  std::string::const_iterator const& c,
  std::string const& arg
);
static char expand_escape_sequence(
  std::string::const_iterator& c
);

static auto const ditto_marker = std::string { 1, '\0' };

bool is_ditto(std::string const& str) {
  return str == ditto_marker;
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

  auto expanded = std::string { };
  auto insert = std::back_inserter(expanded);

  for (auto c = str.begin(); c != str.end(); ++c) {
    if (*c == '&') {
      if (!expanded.empty()) {
        replacements.emplace_back(expanded);
        expanded.clear();
      }

      replacements.push_back(ditto_marker);
      continue;
    }

    auto is_escape = stiX::is_valid_escape_sequence(c, str.end());

    insert = is_escape ? expand_escape_sequence(c) : *c;
  }

  if (!expanded.empty())
    replacements.emplace_back(expanded);

  return replacements;
}

char expand_escape_sequence(
  std::string::const_iterator& c
) {
  auto candidate = *++c;
  return stiX::expand_escape(candidate);
} // expand_escape_sequence