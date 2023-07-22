
#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include "command_parser.hpp"

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void do_append(std::istream& in, size_t before, edit_buffer& buffer);
void do_insert(std::istream& in, size_t before, edit_buffer& buffer);
void do_print(std::ostream& out, size_t from, size_t to, edit_buffer& buffer);

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto parsed_command = parse_command(line);
    auto command = parsed_command.compile(buffer_);

    buffer_.set_dot(command.dot);
    switch(command.code) {
      case '=':
        out << buffer_.dot() << "\n";
        break;
      case 'a':
        do_append(
          in,
          command.to_index,
          buffer_
        );
        break;
      case 'i':
        do_insert(
          in,
          command.to_index,
          buffer_
        );
        break;
      case 'p':
        do_print(
          out,
          command.from_index,
          command.to_index,
          buffer_
        );
        break;
      default:
        out << "?\n";
        break;
    }
  }
}

void do_append(std::istream& in, size_t after, edit_buffer& buffer) {
  auto adjust = (!buffer.empty()) ? 1 : 0;

  do_insert(in, after+adjust, buffer);
}
void do_insert(std::istream& in, size_t before, edit_buffer& buffer) {
  auto adjust = (!buffer.empty()) ? 1 : 0;

  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (line == ".")
      return;

    buffer.insert_before(before-adjust, line);
    ++before;
  }
}

void do_print(std::ostream& out, size_t from, size_t to, edit_buffer& buffer) {
  auto index = buffer.dot();

  if (index == 0) {
    out << "?\n";
    return;
  }

  for (auto index = from; index <= to; ++index)
    out << buffer.line_at(index) << '\n';
}