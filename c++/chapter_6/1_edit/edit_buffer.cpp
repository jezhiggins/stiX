#include "edit_buffer.hpp"

using namespace stiX;

void edit_buffer::insert(size_t index, std::string_view line) {
  buffer_.insert(index, line);
  dot_ = index+1;
}

void edit_buffer::set_at(size_t index, std::string_view line) {
  buffer_.set(index-1, line);
}

void edit_buffer::remove_at(size_t index) {
  buffer_.remove(index-1);
  dot_ = std::min(index, buffer_.size());
}

void edit_buffer::swap(size_t lindex, size_t rindex) {
  auto rhs = std::string { line_at(rindex) };
  if (lindex != 0) {
    auto lhs = std::string { line_at(lindex) };
    set_at(lindex, rhs);
    set_at(rindex, lhs);
  } else {
    remove_at(rindex);
    insert(0, rhs);
  }
}

std::string_view edit_buffer::line_at(size_t index) const {
  // editor is 1-based, underlying buffer is 0-based
  return buffer_[index-1];
}
