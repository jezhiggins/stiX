#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include <sstream>
#include "command_parser.hpp"

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void editor::load(std::string_view filename, std::ostream& out) {
  auto cmd = std::string("e ");
  auto parsed_load_command = parse_command(cmd.append(filename));
  auto load_command = parsed_load_command.compile(buffer_);

  auto null_stream = std::istringstream();
  load_command(null_stream, out, buffer_, filename_);
}

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto parsed_command = parse_command(line);
    auto command = parsed_command.compile(buffer_);

    command(in, out, buffer_, filename_);
  }
}
