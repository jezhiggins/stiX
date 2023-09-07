
#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include "command_parser.hpp"
#include "command_actions.hpp"

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void editor::process(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto parsed_command = parse_command(line);
    auto command = parsed_command.compile(buffer_);

    buffer_.set_dot(command.dot);
    switch(command.code) {
      case '=':
        do_current_line(out, buffer_);
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
        command(in, out, buffer_);
        break;
    }
  }
}
