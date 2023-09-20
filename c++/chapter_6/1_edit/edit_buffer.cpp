#include "edit_buffer.hpp"

using namespace stiX;

void edit_buffer::insert_before(size_t index, std::string_view line) {
  buffer_.insert(index, line);
  dot_ = index+1;
}

void edit_buffer::remove_at(size_t index) {
  buffer_.remove(index-1);
  dot_ = std::min(index, buffer_.size());
}

std::string_view edit_buffer::line_at(size_t index) const {
  // editor is 1-based, underlying buffer is 0-based
  return buffer_[index-1];
}
