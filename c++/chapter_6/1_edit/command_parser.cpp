#include "command_parser.hpp"

stiX::command stiX::parse_command(std::string_view input, size_t dot, size_t last) {
  auto code = input.size() != 0 ? input.front() : '\n';

  return command(dot, dot, code);
}