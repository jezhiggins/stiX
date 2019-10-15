#include "translit.hpp"

#include "../../lib/filter.h"
#include <string>
#include <algorithm>

namespace {
    void transliterate(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    );
    void transliterateWithSquash(
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
  if (with.empty())
      removeCharacters(replace, in, out);
  else if (replace.size() > with.size())
      transliterateWithSquash(replace, with, in, out);
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
                : c;;
            }
        );
    } // transliterate

    class translitterAndSquash {
    public:
        translitterAndSquash(
            const std::string& replace,
            const std::string& with
        )
        : replace_(replace)
        , with_(with)
        , boundary_(with.size() - 1){
            while(with_.size() != replace_.size())
                with_.push_back(with_[boundary_]);
        } // translitterAndSquash

        std::string operator()(char c) {
            auto replacement = replace_.find(c);

            if (replacement == std::string::npos) {
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
            return std::string(1, with_[replacement]);
        } // operator

    private:
        const std::string& replace_;
        std::string with_;
        std::string::size_type boundary_;
        bool squashing_ = false;
    }; // translitterAndSquash

    void transliterateWithSquash(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    ) {
        stiX::filter(
            in,
            out,
            translitterAndSquash(replace, with)
        );
    }

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