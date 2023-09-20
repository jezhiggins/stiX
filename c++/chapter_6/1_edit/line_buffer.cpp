#include "line_buffer.hpp"
#include <string_view>

using namespace stiX;

void line_buffer::insert(size_t index, std::string_view line) {
  lines_.insert(lines_.begin() + index, std::string { line });
}

void line_buffer::remove(size_t index) {
  lines_.erase(lines_.begin() + index);
}

std::string_view line_buffer::operator[](size_t index) const {
  return lines_[index];
}