#ifndef STICPP_COMPARE_HPP
#define STICPP_COMPARE_HPP

#include <iosfwd>
#include <string>

namespace stiX {
    void compare(
        std::string left_file_name,
        std::istream& left,
        std::string right_file_name,
        std::istream& right,
        std::ostream& out
    );
}

#endif //STICPP_COMPARE_HPP
