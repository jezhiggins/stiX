#include "command_actions.hpp"
#include "../../lib/getline.hpp"
#include "edit_buffer.hpp"
#include <fstream>

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void stiX::current_line_action(std::istream&, std::ostream& out, edit_buffer& buffer, std::string&) {
  out << buffer.dot() << '\n';
}

namespace {
  void read_lines(
      std::istream& source,
      size_t index,
      bool stop_on_stop,
      edit_buffer& buffer) {
    while(source.peek() != eof) {
      auto line = stiX::getline(source);

      if (stop_on_stop && line == ".")
        return;

      buffer.insert(index, line);
      ++index;
    }
  }

  void write_lines(std::ostream& out, size_t from, size_t to, edit_buffer& buffer) {
    for (auto index = from; index <= to; ++index)
      out << buffer.line_at(index) << '\n';
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
        {after+1, to},
        {after+1, after+offset+1},
        {after+offset+2, to}
    };
  }
}

void stiX::append_action(std::istream& in, size_t after, edit_buffer& buffer) {
  read_lines(in, after, true, buffer);
}

void stiX::insert_action(std::istream& in, size_t before, edit_buffer& buffer) {
  auto adjust = !buffer.empty() ? 1 : 0;
  read_lines(in, before-adjust, true, buffer);
}

void stiX::change_action(std::istream& in, size_t from, size_t to, edit_buffer& buffer) {
  delete_action(from, to, buffer);
  insert_action(in, !buffer.empty() ? from : 0, buffer);
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
  if (buffer.empty()) {
    out << "?\n";
    return;
  }

  buffer.set_dot(to);
  write_lines(out, from, to, buffer);
}

void stiX::filename_action(std::string filename, std::string& property, std::ostream& out) {
  if (!filename.empty())
    property = filename;

  out << (!property.empty() ? property : "?") << "\n";
}

void stiX::write_to_file_action(size_t from, size_t to, std::string filename, std::string& property, edit_buffer& buffer) {
  if (!filename.empty())
    property = filename;

  auto destination = std::ofstream(property);
  write_lines(destination, from, to, buffer);
}

void stiX::read_from_file_action(size_t before, std::string filename, std::string& property, edit_buffer& buffer) {
  if (!filename.empty())
    property = filename;

  auto source = std::ifstream(property);
  read_lines(source, before, false, buffer);
}

void stiX::edit_file_action(std::string filename, std::string& property, edit_buffer& buffer) {
  if (!filename.empty())
    property = filename;

  if (!buffer.empty())
    delete_action(1, buffer.last(), buffer);
  read_from_file_action(0, filename, property, buffer);
}

void stiX::error_action(std::istream&, std::ostream& out, edit_buffer&, std::string&) {
  out << "?\n";
}
