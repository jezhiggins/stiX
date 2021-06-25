#ifndef STICPP_PATTERN_MATCHER_HPP
#define STICPP_PATTERN_MATCHER_HPP

#include "matcher.hpp"
#include <vector>

namespace stiX {
  class pattern_matcher {
  public:
    bool match(const std::string& line) const;
    size_t size() const { return m_.size(); }

  private:
    explicit pattern_matcher(std::vector<matcher> m);

    std::vector<matcher> m_;

    friend pattern_matcher compile_pattern(const std::string&);
  };

  pattern_matcher compile_pattern(const std::string& pattern);
}
#endif //STICPP_PATTERN_MATCHER_HPP
