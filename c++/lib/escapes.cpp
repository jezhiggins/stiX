#include "escapes.hpp"
#include <map>

char stiX::expand_escape(char candidate) {
  static const std::map<char, char> escapes = {
    { 't', '\t' },
    { 'n', '\n' }
  }; // escapes

  auto escape = escapes.find(candidate);

  return escape != escapes.end()
         ? escape->second
         : candidate;
}

char stiX::is_valid_escape_sequence(
  std::string::const_iterator const& c,
  std::string::const_iterator const& end
) {
  if (*c != Escape)
    return false;

  return (c+1 != end);
} // is_valid_escape_sequence
