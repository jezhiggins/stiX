#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include <sstream>
#include "command_parser.hpp"

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void editor::load(std::string_view filename, std::ostream& out) {
  auto load_file = std::string("e ").append(filename);
  auto null_stream = std::istringstream();

  handle_command(load_file, null_stream, out);
}

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    handle_command(line, in, out);
  }
}

void editor::handle_command(std::string_view line, std::istream&in, std::ostream&out) {
  auto parsed_command = parse_command(line);
  auto commands = parsed_command.compile(buffer_);

  commands(in, out, buffer_);
}

