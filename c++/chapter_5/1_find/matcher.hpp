#ifndef STICPP_MATCHER_HPP
#define STICPP_MATCHER_HPP

#include <string>

namespace stiX {
  class matcher {
  public:
    bool match(char candidate) const;

  private:
    explicit matcher(std::string  character);

    std::string pattern_;

    friend matcher make_matcher(const std::string& character);
  };

  matcher make_matcher(const std::string& character);
}

#endif //STICPP_MATCHER_HPP
