#include "edit_buffer.hpp"

using namespace stiX;

void edit_buffer::insert_before(size_t index, std::string_view line) {
  buffer_.insert(index, line);
  dot_ = index+1;
}

std::string_view edit_buffer::operator[](size_t index) const {
  return buffer_[index];
}
