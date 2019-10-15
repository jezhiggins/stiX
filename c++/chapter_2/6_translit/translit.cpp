#include "translit.hpp"

#include "../../lib/filter.h"
#include <string>
#include <algorithm>

namespace {
    void transliterate(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    );
    void removeCharacters(
        const std::string& remove,
        std::istream& in, std::ostream& out
    );
}

void stiX::translit(
    const std::string& replace, const std::string& with,
    std::istream& in, std::ostream& out
) {
  if (!with.empty())
      transliterate(replace, with, in, out);
  else
      removeCharacters(replace, in, out);
} // stiX::translit

namespace {
    void transliterate(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    ) {
        stiX::raw_filter(
            in,
            out,
            [&replace, &with](char c) {
                auto replacement = replace.find(c);
                return replacement != std::string::npos
                ? with[replacement]
                : c;;
            }
        );
    } // transliterate

    void removeCharacters(
        const std::string& remove,
        std::istream& in, std::ostream& out
    ) {
        std::remove_copy_if(
            std::istreambuf_iterator(in),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator(out),
            [&remove](char c) {
                return remove.find(c) != std::string::npos;
            }
        );
    }
} // namespace