#ifndef STICPP_PATTERN_MATCHER_HPP
#define STICPP_PATTERN_MATCHER_HPP

#include "matcher.hpp"
#include <vector>

namespace stiX {
  enum class match_count {
    one,
    zero_or_more
  };
  struct pattern {
    matcher test;
    match_count count;

    pattern() = delete;
    explicit pattern(matcher t);
  };

  using patterns = std::vector<pattern>;

  class pattern_matcher {
  public:
    bool match(std::string const& line) const;
    size_t size() const { return m_.size(); }

  private:
    explicit pattern_matcher(patterns m);

    patterns const m_;

    friend pattern_matcher compile_pattern(std::string const&);
  };

  pattern_matcher compile_pattern(std::string const& pattern);
}
#endif //STICPP_PATTERN_MATCHER_HPP
