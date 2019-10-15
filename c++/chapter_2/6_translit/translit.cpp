#include "translit.hpp"

#include "../../lib/filter.h"
#include <string>

void stiX::translit(
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
