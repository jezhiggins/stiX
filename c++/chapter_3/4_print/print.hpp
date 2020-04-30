#ifndef STICPP_PRINT_HPP
#define STICPP_PRINT_HPP

#include <iosfwd>
#include <string>

namespace stiX {
  int print(
    std::string const& filename,
    std::istream& input,
    std::ostream& output,
    size_t page_length = 64
  );
}

#endif