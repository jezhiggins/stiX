#include "translit-args.hpp"

#include <cctype>

namespace {
    const char dash = '-';

    bool is_valid_dash_range(
        const std::string& arg,
        const std::string::const_iterator& c
    );

    template<typename Dest, typename Src>
    void expand_dash_range(Dest& dest, Src& c);
}

std::string stiX::translitArgument(
    const std::string &arg
) {
  std::string expanded;
  auto insert = std::back_inserter(expanded);
  auto c = arg.begin();
  while (c != arg.end()) {
    if (*c != dash) {
      *insert++ = *c++;
      continue;
    }
    if (is_valid_dash_range(arg, c)) {
      expand_dash_range(insert, c);
      continue;
    }
    *insert++ = *c++;
  }
  return expanded;
}

namespace {
    bool is_valid_dash_range(
        const std::string &arg,
        const std::string::const_iterator &c
    ) {
      auto prev = *(c - 1);
      auto next = *(c + 1);

      return
          std::isalnum(prev)
          && std::isalnum(next)
          && (prev < next);
    } // in_valid_dash_range

    template<typename Dest, typename Src>
    void expand_dash_range(Dest &dest, Src &c) {
      char from = *(c - 1) + 1;
      char to = *++c;

      while (from <= to)
        *dest++ = from++;

      c++;
    } // expand_dash_range
}
