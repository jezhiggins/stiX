#ifndef STICPP_PRINT_HPP
#define STICPP_PRINT_HPP

#include <iosfwd>
#include <vector>

namespace stiX {
  void print_files(
    std::istream& archive,
    std::vector<std::string> const& files,
    std::ostream& out
  );
}

#endif //STICPP_PRINT_HPP
