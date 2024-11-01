#include <locale>
#include <algorithm>
#include "chars.hpp"

const std::locale loc = std::locale();

bool stiX::isdigit(char c) { return std::isdigit(c, loc); }
bool stiX::isalnum(char c) { return std::isalnum(c, loc); }
bool stiX::iswhitespace(char c) { return std::isspace(c, loc); }

bool stiX::isalnum(std::string_view s) {
  if (!std::isalpha(*s.begin(), loc))
    return false;

  return std::all_of(s.begin()+1, s.end(), [](char c) { return stiX::isalnum(c); });
}

bool stiX::iswhitespace(std::string_view s) {
  if (!iswhitespace(*s.begin()))
    return false;

  return std::all_of(s.begin()+1, s.end(), [](char c) { return iswhitespace(c); });
}