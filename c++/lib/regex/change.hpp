#ifndef STICPP_CHANGE_HPP
#define STICPP_CHANGE_HPP

#include <ios>
#include <string>

namespace stiX {
  class pattern_matcher;
  class replacement;

  void change(
    std::istream& in,
    std::ostream& out,
    std::string const& pattern,
    std::string const& replacement);

  void apply_change(
    pattern_matcher const& matcher,
    replacement const& replacer,
    std::string_view line,
    std::ostream& out
  );

  void apply_change_once(
    pattern_matcher const& matcher,
    replacement const& replacer,
    std::string_view line,
    std::ostream& out
  );
}

#endif //STICPP_CHANGE_HPP
