#include "translit-args.hpp"

#include <cctype>
#include <map>
#include "../../lib/escapes.hpp"

namespace {
    const char Caret = '^';
    const char Dash = '-';

    std::string translitArgument(
        const std::string &arg,
        bool ignoreInitialCaret
    );

    bool is_valid_dash_range(
        const std::string& arg,
        const std::string::const_iterator& c
    );

    void expand_dash_range(
        std::back_insert_iterator<std::string>& dest,
        std::string::const_iterator& c
    );

    bool is_valid_escape_sequence(
        const std::string& arg,
        const std::string::const_iterator& c
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
    const std::string &arg
) {
  return translitArgument(arg, true);
}

std::string stiX::translitDestArgument(
    const std::string &arg
) {
  return translitArgument(arg, false);
}

namespace {
    std::string translitArgument(
        const std::string &arg,
        bool ignoreInitialCaret
    ) {
      std::string expanded;
      auto insert = std::back_inserter(expanded);

      auto c = arg.begin();
      if (ignoreInitialCaret && *c == Caret)
        ++c;

      while (c != arg.end()) {
        if (is_valid_escape_sequence(arg, c)) {
          expand_escape_sequence(insert, c);
          continue;
        }
        if (is_valid_dash_range(arg, c)) {
          expand_dash_range(insert, c);
          continue;
        }

        insert = *c++;
      }
      return expanded;
    } // translitArgument

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

    void expand_dash_range(
        std::back_insert_iterator<std::string>& dest,
        std::string::const_iterator &c) {
      char from = *(c - 1) + 1;
      char to = *++c;

      while (from <= to)
        dest = from++;

      c++;
    } // expand_dash_range

    bool is_valid_escape_sequence(
        const std::string &arg,
        const std::string::const_iterator &c
    ) {
      if (*c != stiX::Escape)
        return false;

      return (c+1 != arg.end());
    } // is_valid_escape_sequence

    void expand_escape_sequence(
        std::back_insert_iterator<std::string>& dest,
        std::string::const_iterator& c
    ) {
      static const std::map<char, char> escapes = {
          { 't', '\t' },
          { 'n', '\n' }
      }; // escapes

      auto candidate = *++c;
      auto escaped = stiX::expand_escape(candidate);

      dest = escaped;

      ++c;
    } // expand_escape_sequence
} // namespace
