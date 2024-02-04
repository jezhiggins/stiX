#ifndef STICPP_CHANGE_HPP
#define STICPP_CHANGE_HPP

#include <ios>
#include <string>

namespace stiX {
  void change(
    std::istream& in,
    std::ostream& out,
    std::string const& pattern,
    std::string const& replacement);
}

#endif //STICPP_CHANGE_HPP
