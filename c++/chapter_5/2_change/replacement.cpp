
#include "replacement.hpp"
#include "../../lib/escapes.hpp"

static bool is_valid_escape_sequence(
  std::string::const_iterator const& c,
  std::string const& arg
);
static char expand_escape_sequence(
  std::string::const_iterator& c
);

std::string stiX::prepare_replacement(std::string const& str) {
  std::string expanded;
  auto insert = std::back_inserter(expanded);

  for (auto c = str.begin(); c != str.end(); ++c) {
    auto is_escape = stiX::is_valid_escape_sequence(c, str.end());

    insert = is_escape ? expand_escape_sequence(c) : *c;
  }
  return expanded;
}

char expand_escape_sequence(
  std::string::const_iterator& c
) {
  auto candidate = *++c;
  return stiX::expand_escape(candidate);
} // expand_escape_sequence
