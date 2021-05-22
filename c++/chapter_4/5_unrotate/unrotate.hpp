#ifndef STICPP_UNROTATE_HPP
#define STICPP_UNROTATE_HPP

#include <string>

namespace stiX {
  void unrotate(std::istream& in, std::ostream& out, size_t line_length, char fold_marker);
  std::string unrotate_line(std::string const& input_line, size_t output_line_length, char fold_marker);
}
#endif // STICPP_UNROTATE_HPP

