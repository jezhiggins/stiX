
#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include "command_parser.hpp"

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void do_insert(std::istream& in, edit_buffer& buffer);
void do_print(std::ostream& out, edit_buffer& buffer);

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto command = parse_command(
      line,
      buffer_.dot(),
      buffer_.last());

    switch(command.code) {
      case '=':
        out << buffer_.dot() << "\n";
        break;
      case 'i':
        do_insert(in, buffer_);
        break;
      case 'p':
        do_print(out, buffer_);
        break;
      default:
        out << "?\n";
        break;
    }
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