
#include "compare.hpp"
#include <iostream>
#include "../../lib/getline.h"

namespace {
    const std::string end_of_file = "end of file on ";
}

namespace stiX {
    void compare(
        std::string left_file_name,
        std::istream& left,
        std::string right_file_name,
        std::istream& right,
        std::ostream& out
    ) {
        size_t line_count = 1;
        do {
            auto left_line = getline(left);
            auto right_line = getline(right);

            if (left.eof() && !right.eof()) {
                out << end_of_file << left_file_name << '\n';
                return;
            }
            if (!left.eof() && right.eof()) {
                out << end_of_file << right_file_name << '\n';
                return;
            }

            if (left_line != right_line) {
                out << line_count << ':' << '\n'
                    << left_line << '\n'
                    << right_line << '\n';
            }

            ++line_count;
        } while (left && right);
    } // compare
}