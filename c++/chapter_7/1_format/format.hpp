#ifndef STICPP_FORMAT_HPP
#define STICPP_FORMAT_HPP

#include <iosfwd>

namespace stiX {
  class formatter {
  public:
    void operator()(std::istream &in, std::ostream &out);
  };
}

#endif