#include "line_buffer.hpp"
#include <string_view>

using namespace stiX;

void line_buffer::insert(size_t index, std::string_view line) {
  lines_.insert(lines_.begin() + index, std::string { line });
}

void line_buffer::set(size_t index, std::string_view line) {
  lines_[index] = line;
}


void line_buffer::remove(size_t index) {
  lines_.erase(lines_.begin() + index);
}

void line_buffer::swap(size_t lindex, size_t rindex) {
  auto rhs = lines_[rindex];

  if (lindex != -1) {
    auto lhs = lines_[lindex];
    lines_[lindex] = rhs;
    lines_[rindex] = lhs;
  } else {
    lines_.erase(lines_.begin() + rindex);
    lines_.insert(lines_.begin(), rhs);
  }
}

std::string_view line_buffer::operator[](size_t index) const {
  return lines_[index];
}