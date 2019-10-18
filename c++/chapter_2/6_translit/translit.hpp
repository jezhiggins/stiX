#ifndef STICPP_TRANSLIT_H
#define STICPP_TRANSLIT_H

#include <iosfwd>

namespace stiX {
    enum class Selection : bool {
        Normal,
        Negate
    };

    void translit(
        const std::string& replace,
        const std::string& with,
        Selection mode,
        std::istream& in,
        std::ostream& out
    );
}

#endif //STICPP_TRANSLIT_H
