#include "edit_buffer.hpp"
#include <string_view>

using namespace stiX;

void edit_buffer::insert(size_t index, std::string_view line) {
  lines_.insert(lines_.begin() + (index-1), std::string { line });
}