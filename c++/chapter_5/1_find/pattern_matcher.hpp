#ifndef STICPP_PATTERN_MATCHER_HPP
#define STICPP_PATTERN_MATCHER_HPP

#include "matcher.hpp"
#include <vector>

namespace stiX {
  enum class match_count {
    one,
    zero_or_more
  };
  struct match_stage {
    matcher test;
    match_count count;

    match_stage() = delete;
    explicit match_stage(matcher t);
  };

  using match_stages = std::vector<match_stage>;

  class pattern_matcher {
  public:
    bool match(std::string const& line) const;
    size_t size() const { return m_.size(); }

  private:
    explicit pattern_matcher(match_stages m);

    match_stages const m_;

    friend pattern_matcher compile_pattern(std::string const&);
  };

  pattern_matcher compile_pattern(std::string const& pattern);
}
#endif //STICPP_PATTERN_MATCHER_HPP
