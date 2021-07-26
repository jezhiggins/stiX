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