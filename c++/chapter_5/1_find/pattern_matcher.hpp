#ifndef STICPP_PATTERN_MATCHER_HPP
#define STICPP_PATTERN_MATCHER_HPP

#include "matcher.hpp"

namespace stiX {
  class pattern_matcher {
  public:
    bool match(const std::string& line) const {
      return m_.match(line[0]);
    }
    size_t size() const { return 1; }

  private:
    explicit pattern_matcher(matcher m);

    matcher m_;

    friend pattern_matcher compile_pattern(const std::string&);
  };

  pattern_matcher compile_pattern(const std::string& pattern);
}
#endif //STICPP_PATTERN_MATCHER_HPP
