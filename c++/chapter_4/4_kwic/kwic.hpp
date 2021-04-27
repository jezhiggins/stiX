#ifndef STICPP_KWIC_HPP
#define STICPP_KWIC_HPP

#include <vector>
#include <string>

namespace stiX {
  void kwic(std::istream& in, std::ostream& out);
  std::vector<std::string> kwic_line(std::string const &line);
}
#endif //STICPP_KWIC_HPP
