#include "translit-args.hpp"

#include <cctype>
#include <map>

namespace {
    const char Dash = '-';
    const char Escape = '@';

    bool is_valid_dash_range(
        const std::string& arg,
        const std::string::const_iterator& c
    );

    template<typename Dest, typename Src>
    void expand_dash_range(Dest& dest, Src& c);

    bool is_valid_escape_sequence(
        const std::string& arg,
        const std::string::const_iterator& c
    );

    template<typename Dest, typename Src>
    void expand_escape_sequence(Dest& dest, Src& c);
}

std::string stiX::translitArgument(
    const std::string &arg
) {
  std::string expanded;
  auto insert = std::back_inserter(expanded);

  auto c = arg.begin();
  while (c != arg.end()) {
    if (is_valid_escape_sequence(arg, c)) {
      expand_escape_sequence(insert, c);
      continue;
    }
    if (*c != Dash) {
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
      if (*c != Dash)
        return false;

      if ((c == arg.begin())
       || (c+1 == arg.end()))
        return false;

      auto prev = *(c - 1);
      auto next = *(c + 1);

      if (prev >= next)
        return false;

      return
          (std::isdigit(prev) && std::isdigit(next))
       || (std::islower(prev) && std::islower(next))
       || (std::isupper(prev) && std::isupper(next));
    } // in_valid_dash_range

    template<typename Dest, typename Src>
    void expand_dash_range(Dest &dest, Src &c) {
      char from = *(c - 1) + 1;
      char to = *++c;

      while (from <= to)
        *dest++ = from++;

      c++;
    } // expand_dash_range

    bool is_valid_escape_sequence(
        const std::string &arg,
        const std::string::const_iterator &c
    ) {
      if (*c != Escape)
        return false;

      return (c+1 != arg.end());
    } // is_valid_escape_sequence

    template<typename Dest, typename Src>
    void expand_escape_sequence(Dest &dest, Src &c) {
      static const std::map<char, char> escapes = {
          { 't', '\t' },
          { 'n', '\n' }
      }; // expand_dash_range

      auto candidate = *++c;
      auto escape = escapes.find(candidate);

      *dest++ = escape != escapes.end()
          ? escape->second
          : candidate;

      ++c;
    } // expand_escape_sequence
} // namespace
