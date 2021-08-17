#include "dash_range.hpp"

#include <cctype>

bool stiX::is_dash_range(char from, char to) {
  if (from > to)
    return false;

  return (std::isdigit(from) && std::isdigit(to))
    || (std::islower(from) && std::islower(to))
    || (std::isupper(from) && std::isupper(to));
}