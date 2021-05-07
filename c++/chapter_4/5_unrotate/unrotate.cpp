#include "unrotate.hpp"

std::string stiX::unrotateLine(std::string const& line, size_t line_length, char fold_marker) {
  auto fold_position = line.find(fold_marker);
  if (fold_position == std::string::npos)
    return line;

  auto half_length = line_length / 2;
  auto remainder = line.length() - fold_position;
  auto output =
    std::string(half_length - remainder, ' ') +
    line.substr(fold_position + 1) +
    "  " +
    line.substr(0, fold_position);
  return output;
}
