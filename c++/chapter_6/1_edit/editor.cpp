#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include "command_parser.hpp"

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto parsed_command = parse_command(line);
    auto command = parsed_command.compile(buffer_);

    buffer_.set_dot(command.dot);

    command(in, out, buffer_);
  }
}
