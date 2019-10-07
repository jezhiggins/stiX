#ifndef STICPP_TRANSLIT_H
#define STICPP_TRANSLIT_H

#include <iosfwd>

namespace stiX {
    void translit(
        const std::string& replace, const std::string& with,
        std::istream& in, std::ostream& out
    );
}

#endif //STICPP_TRANSLIT_H
