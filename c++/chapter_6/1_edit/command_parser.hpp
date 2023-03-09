#ifndef STICPP_COMMAND_PARSER_H
#define STICPP_COMMAND_PARSER_H

#include <string_view>
#include <functional>

namespace stiX {
  class lines;
  using index_fn = std::function<size_t(lines const&)>;

  struct command {
    index_fn const from_index = line_error_fn;
    index_fn const to_index = line_error_fn;
    char const code = code_error;

    static size_t const line_error;
    static char const code_error;

    static command const error;
  private:
    static size_t line_error_fn(lines const&);
  };

  command parse_command(std::string_view input);
}

#endif //STICPP_COMMAND_PARSER_H
