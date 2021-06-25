#include "pattern_matcher.hpp"

stiX::pattern_matcher::pattern_matcher(stiX::matcher m)
  : m_(std::move(m)){
}



stiX::pattern_matcher stiX::compile_pattern(const std::string& pattern) {
  auto matcher = stiX::make_matcher(pattern.substr(0, 1));
  return pattern_matcher(matcher);
}
