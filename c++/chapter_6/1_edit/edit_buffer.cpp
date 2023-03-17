#include "edit_buffer.hpp"

using namespace stiX;

void edit_buffer::insert_before(size_t index, std::string_view line) {
  buffer_.insert(index, line);
  dot_ = index+1;
}

std::string_view edit_buffer::line_at(size_t index) const {
  // editor is 1-based, underlying buffer is 0-based
  return buffer_[index-1];
}
