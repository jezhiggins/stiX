#ifndef STICPP_FORMAT_FUNCTIONS_HPP
#define STICPP_FORMAT_FUNCTIONS_HPP

#include <string>

namespace stiX {
  std::string fill_line(std::string const& line_in, size_t width);
  std::string centre_line(std::string const& line_in, size_t width);
  std::string underline(std::string_view line_in);
}

#endif //STICPP_FORMAT_FUNCTIONS_HPP
