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

  struct match_location {
    using size_type = std::string_view::size_type;

    bool const match;
    size_type const from;
    size_type const to;
  };

  class pattern_matcher {
  public:
    match_location find(std::string_view line) const;
    bool match(std::string_view line) const;

    size_t size() const { return m_.size(); }

  private:
    explicit pattern_matcher(patterns m);

    bool is_terminal_only() const;

    patterns const m_;

    friend pattern_matcher compile_pattern(std::string_view);
  };

  pattern_matcher compile_pattern(std::string_view pattern);
}
#endif //STICPP_PATTERN_MATCHER_HPP