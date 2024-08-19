#ifndef STICPP_FILLER_HPP
#define STICPP_FILLER_HPP

#include <string>

namespace stiX {
  class filler {
    public:
      std::string operator()(std::string const &line_in, size_t width);

    private:
      bool direction_;
  };
}

#endif //STICPP_FILLER_HPP
