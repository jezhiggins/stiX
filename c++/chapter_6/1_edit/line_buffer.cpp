#include "line_buffer.hpp"
#include <string_view>

using namespace stiX;

void line_buffer::insert(size_t index, std::string_view line) {
  lines_.insert(lines_.begin() + index, std::string { line });
}

std::string_view line_buffer::operator[](size_t index) const {
  return lines_[index];
}