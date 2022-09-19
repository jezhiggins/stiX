
#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void do_insert(std::istream& in, edit_buffer& buffer);
void do_print(std::ostream& out, edit_buffer& buffer);

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (line == "=")
      out << buffer_.dot() << "\n";
    else if (line == "i")
      do_insert(in, buffer_);
    else if (line == "p")
      do_print(out, buffer_);
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

void do_print(std::ostream& out, edit_buffer& buffer) {
  auto index = buffer.dot();

  if (index == 0)
    out << "?";
  else
    out << buffer[index-1];

  out << "\n";
}