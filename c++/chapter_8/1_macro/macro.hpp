#ifndef STICPP_MACRO_HPP
#define STICPP_MACRO_HPP

#include <iosfwd>

namespace stiX {
  class macro_processor {
  public:
    macro_processor();

    void process(std::istream& in, std::ostream& out);
  };
}

#endif // STICPP_MACRO_HPP