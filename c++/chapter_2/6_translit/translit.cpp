#include "translit.hpp"

#include "../../lib/filter.hpp"
#include <string>
#include <algorithm>
#include <functional>

namespace {
    typedef std::function<bool(std::string::size_type)> Matcher;

    void transliterate(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    );
    void transliterateWithSquash(
        const std::string& replace, const std::string& with,
        Matcher matchFound,
        std::istream& in, std::ostream& out
    );
    void removeCharacters(
        const std::string& remove,
        Matcher matchFound,
        std::istream& in, std::ostream& out
    );

    bool in_range(std::string::size_type p) {
        return p != std::string::npos;
    }
    bool out_of_range(std::string::size_type p) {
        return p == std::string::npos;
    }
}

void stiX::translit(
    const std::string& replace,
    const std::string& with,
    stiX::Selection selectionMode,
    std::istream& in,
    std::ostream& out
) {
  auto matcher = (selectionMode == stiX::Selection::Normal)
      ? in_range
      : out_of_range;

  if (with.empty())
      removeCharacters(replace, matcher, in, out);
  else if (replace.size() > with.size())
      transliterateWithSquash(replace, with, matcher, in, out);
  else
      transliterate(replace, with, in, out);
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
                    : c;
            }
        );
    } // transliterate

    class translitterAndSquash {
    public:
        translitterAndSquash(
            const std::string& replace,
            const std::string& with,
            Matcher matchFound
        )
        : replace_(replace)
        , with_(with)
        , matchFound_(matchFound)
        , boundary_(with.size() - 1)
        , squash_(*with.rbegin()) {
        } // translitterAndSquash

        std::string operator()(char c) {
            auto replacement = replace_.find(c);

            if (!matchFound_(replacement)) {
                squashing_ = false;
                return std::string(1, c);
            } // nothing to do

            if (
                (replacement >= boundary_)
                && squashing_
            ) {
                return std::string();
            } // already squashing - do nothing

            squashing_ = (replacement >= boundary_);
            return std::string(
                1,
                squashing_ ? squash_ : with_[replacement]
            );
        } // operator

    private:
        const std::string& replace_;
        std::string with_;
        Matcher matchFound_;
        const char squash_;
        std::string::size_type boundary_;
        bool squashing_ = false;
    }; // translitterAndSquash

    void transliterateWithSquash(
        const std::string& replace, const std::string& with,
        Matcher matchFound,
        std::istream& in, std::ostream& out
    ) {
        stiX::filter(
            in,
            out,
            translitterAndSquash(replace, with, matchFound)
        );
    }

    void removeCharacters(
        const std::string& remove,
        Matcher matchFound,
        std::istream& in, std::ostream& out
    ) {
        std::remove_copy_if(
            std::istreambuf_iterator(in),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator(out),
            [&remove, &matchFound](char c) {
                return matchFound(remove.find(c));
            }
        );
    }
} // namespace