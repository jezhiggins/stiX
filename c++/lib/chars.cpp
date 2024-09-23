#include <locale>
#include "chars.hpp"

const std::locale loc = std::locale();

bool stiX::isalnum(char c) {
  return std::isalnum(c, loc);
}