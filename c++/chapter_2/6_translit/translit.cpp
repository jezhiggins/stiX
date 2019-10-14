#include "translit.hpp"

#include "../../lib/filter.h"
#include <string>
#include <cctype>

namespace stiX {
    const char dash = '-';

    bool is_valid_dash_range(
        const std::string& arg,
        const std::string::const_iterator& c
    ) {
        auto prev = *(c-1);
        auto next = *(c+1);

        return
           std::isalnum(prev)
           && std::isalnum(next)
           && (prev < next);
    } // in_valid_dash_range

    template<typename Dest, typename Src>
    void expand_dash_range(Dest& dest, Src& c) {
        char from = *(c-1)+1;
        char to = *++c;

        while (from <= to)
            *dest++ = from++;

        c++;
    }

    std::string translitArgument(
        const std::string& arg
    ) {
        std::string expanded;
        auto insert = std::back_inserter(expanded);
        auto c = arg.begin();
        while(c != arg.end()) {
            if (*c != dash) {
                *insert++ = *c++;
                continue;
            }
            if (is_valid_dash_range(arg, c)) {
               expand_dash_range(insert, c);
               continue;
            }
        }
        return expanded;
    }

    void translit(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    ) {
      raw_filter(
          in,
          out,
          [&replace, &with](char c) {
              auto replacement = replace.find(c);
              return replacement != std::string::npos
                     ? with[replacement]
                     : c;
          }
      );
    }
}
