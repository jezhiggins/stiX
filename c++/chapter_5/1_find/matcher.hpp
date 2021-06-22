#ifndef STICPP_MATCHER_HPP
#define STICPP_MATCHER_HPP

#include <string>
#include <functional>

namespace stiX {
  using match_fn = std::function<bool(char)>;

  class matcher {
  public:
    bool match(char candidate) const;

  private:
    explicit matcher(match_fn fn);

    match_fn fn_;

    friend matcher make_matcher(const std::string&);
  };

  matcher make_matcher(const std::string& characters);
}

#endif //STICPP_MATCHER_HPP
