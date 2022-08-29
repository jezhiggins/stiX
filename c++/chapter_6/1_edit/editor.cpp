#include "editor.hpp"

using namespace stiX;

void editor::insert_before(size_t index, std::string_view line) {
  buffer_.insert(index, line);
  dot_ = index+1;
}