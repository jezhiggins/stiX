
#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void do_insert(std::istream& in, edit_buffer& buffer);

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (line == "=")
      out << buffer_.dot() << "\n";
    else if (line == "i")
      do_insert(in, buffer_);
    else
      out << "?\n";
  }
}

void do_insert(std::istream& in, edit_buffer& buffer) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (line == ".")
      return;

    buffer.insert_before(buffer.dot(), line);
  }
}