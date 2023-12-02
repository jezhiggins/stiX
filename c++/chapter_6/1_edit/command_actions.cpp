#include "command_actions.hpp"
#include "../../lib/getline.hpp"
#include "edit_buffer.hpp"
#include <iostream>

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void stiX::current_line_action(std::istream&, std::ostream& out, edit_buffer& buffer) {
  out << buffer.dot() << "\n";
}

void stiX::append_action(std::istream& in, size_t after, edit_buffer& buffer) {
  auto adjust = !buffer.empty() ? 1 : 0;

  insert_action(in, after+adjust, buffer);
}

void stiX::insert_action(std::istream& in, size_t before, edit_buffer& buffer) {
  auto adjust = !buffer.empty() ? 1 : 0;

  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (line == ".")
      return;

    buffer.insert_before(before-adjust, line);
    ++before;
  }
}

void stiX::change_action(std::istream& in, size_t from, size_t to, edit_buffer& buffer) {
  delete_action(from, to, buffer);
  insert_action(in, !buffer.empty() ? from : 0, buffer);
}

void reverse(size_t from, size_t to, edit_buffer& buffer) {
  auto m = ((to - from) / 2) + 1;
  for (auto i = 0; i != m; ++i)
    buffer.swap(from + i, to - i);
}

std::vector<std::pair<size_t, size_t>> move_pairs(size_t from, size_t to, size_t after) {
  auto offset = to - from;

  if (from < after)
    return std::vector<std::pair<size_t, size_t>> {
          {from, after},
          {after-offset, after},
          {from, after-(offset+1)}
    };

  return std::vector<std::pair<size_t, size_t>> {
      {after, to},
      {after, after+offset},
      {from, to}
  };
}

void stiX::move_action(size_t from, size_t to, size_t after, edit_buffer& buffer) {
  auto moves = move_pairs(from, to, after);
  for (auto [b, e] : moves)
    reverse(b, e, buffer);
}

void stiX::delete_action(size_t from, size_t to, edit_buffer& buffer) {
  do {
    buffer.remove_at(from);
    --to;
  } while(from <= to);
}

void stiX::print_action(std::ostream& out, size_t from, size_t to, edit_buffer& buffer) {
  auto index = buffer.dot();

  if (index == 0) {
    out << "?\n";
    return;
  }

  for (auto index = from; index <= to; ++index)
    out << buffer.line_at(index) << '\n';
}

void stiX::error_action(std::istream&, std::ostream& out, edit_buffer&) {
  out << "?\n";
}

