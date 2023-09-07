#include "command_actions.hpp"
#include "../../lib/getline.hpp"
#include "edit_buffer.hpp"
#include <iostream>

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void stiX::do_current_line(std::ostream &out, stiX::edit_buffer& buffer) {
  out << buffer.dot() << "\n";
}

void stiX::do_append(std::istream& in, size_t after, edit_buffer& buffer) {
  auto adjust = (!buffer.empty()) ? 1 : 0;

  do_insert(in, after+adjust, buffer);
}

void stiX::do_insert(std::istream& in, size_t before, edit_buffer& buffer) {
  auto adjust = (!buffer.empty()) ? 1 : 0;

  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (line == ".")
      return;

    buffer.insert_before(before-adjust, line);
    ++before;
  }
}

void stiX::do_print(std::ostream& out, size_t from, size_t to, edit_buffer& buffer) {
  auto index = buffer.dot();

  if (index == 0) {
    out << "?\n";
    return;
  }

  for (auto index = from; index <= to; ++index)
    out << buffer.line_at(index) << '\n';
}

void stiX::error_action(std::istream&, std::ostream& out, lines&) {
  out << "?\n";
}

