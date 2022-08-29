#include "edit_buffer.hpp"
#include <string_view>

using namespace stiX;

void edit_buffer::insert(size_t index, std::string_view line) {
  lines_.insert(lines_.begin() + index, std::string { line });
}

std::string_view edit_buffer::at(size_t index) const {
  return lines_.at(index);
}