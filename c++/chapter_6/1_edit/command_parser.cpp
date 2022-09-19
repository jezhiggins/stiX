#include "command_parser.hpp"

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  return command(dot, dot, '\n');
}