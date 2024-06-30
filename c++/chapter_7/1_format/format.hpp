#ifndef STICPP_FORMAT_HPP
#define STICPP_FORMAT_HPP

#include <iosfwd>

namespace stiX {
  void format(std::istream& in, std::ostream& out);
  void format(std::istream& in, std::ostream& out, size_t page_width, size_t page_length);
}

#endif