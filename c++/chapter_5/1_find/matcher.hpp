#ifndef STICPP_MATCHER_HPP
#define STICPP_MATCHER_HPP

#include <string>

namespace stiX {
  class matcher {
  public:
    explicit matcher(std::string  character);

    bool match(char candidate) const;

  private:
    std::string pattern_;
  };
}

#endif //STICPP_MATCHER_HPP
