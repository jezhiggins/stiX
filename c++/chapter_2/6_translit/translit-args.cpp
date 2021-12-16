#include "translit-args.hpp"

#include <cctype>
#include <map>
#include "../../lib/escapes.hpp"
#include "../../lib/dash_range.hpp"

namespace {
    const char Caret = '^';

    std::string translitArgument(
        std::string const& arg,
        bool ignoreInitialCaret
    );

    bool is_valid_dash_range(
        std::string::const_iterator const& c,
        std::string const& arg
    );

    void expand_dash_range(
        std::back_insert_iterator<std::string>& dest,
        std::string::const_iterator& c
    );

    void expand_escape_sequence(
        std::back_insert_iterator<std::string>& dest,
        std::string::const_iterator& c
    );
}

bool stiX::isNegatedArgument(
    const std::string& arg
) {
  return arg.size() && arg[0] == Caret;
}

std::string stiX::translitSrcArgument(
    std::string const& arg
) {
  return translitArgument(arg, true);
}

std::string stiX::translitDestArgument(
    std::string const& arg
) {
  return translitArgument(arg, false);
}

namespace {
    std::string translitArgument(
        std::string const& arg,
        bool ignoreInitialCaret
    ) {
      std::string expanded;
      auto insert = std::back_inserter(expanded);

      auto c = arg.begin();
      if (ignoreInitialCaret && *c == Caret)
        ++c;

      while (c != arg.end()) {
        if (stiX::is_valid_escape_sequence(c, arg.end())) {
          expand_escape_sequence(insert, c);
          continue;
        }
        if (is_valid_dash_range(c, arg)) {
          expand_dash_range(insert, c);
          continue;
        }

        insert = *c++;
      }
      return expanded;
    } // translitArgument

    bool is_valid_dash_range(
        std::string::const_iterator const& c,
        std::string const& arg
    ) {
      if (*c != stiX::Dash)
        return false;

      if ((c == arg.begin())
       || (c+1 == arg.end()))
        return false;

      auto prev = *(c - 1);
      auto next = *(c + 1);

      return stiX::is_dash_range(prev, next);
    } // in_valid_dash_range

    void expand_dash_range(
        std::back_insert_iterator<std::string>& dest,
        std::string::const_iterator &c) {
      char from = *(c - 1) + 1;
      char to = *++c;

      stiX::expand_dash_range(from, to, dest);

      c++;
    } // expand_dash_range

    void expand_escape_sequence(
        std::back_insert_iterator<std::string>& dest,
        std::string::const_iterator& c
    ) {
      auto candidate = *++c;
      auto escaped = stiX::expand_escape(candidate);

      dest = escaped;

      ++c;
    } // expand_escape_sequence
} // namespace
