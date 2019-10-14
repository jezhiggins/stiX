#include "translit.hpp"

#include "../../lib/filter.h"
#include <string>

class translitter {
public:
    translitter(const std::string& replace, const std::string& with)
      : replace_(replace)
      , with_(with) {
    }

    char operator()(char c) const {
      auto replacement = replace_.find(c);
      return replacement != std::string::npos
        ? with_[replacement]
        : c;
    }

private:
    std::string replace_;
    std::string with_;
};

namespace stiX {
    void translit(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    ) {
      raw_filter(
          in,
          out,
          translitter(replace, with)
      );
    }
}
