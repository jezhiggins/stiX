#include "matcher.hpp"

#include <utility>

stiX::matcher::matcher(std::string  character)
  : pattern_(std::move(character)) {
}

bool stiX::matcher::match(char candidate) const {
  return pattern_.find(candidate) != std::string::npos;
}
