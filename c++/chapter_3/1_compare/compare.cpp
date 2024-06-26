
#include "compare.hpp"
#include <iostream>
#include "../../lib/getline.hpp"

namespace {
    const std::string end_of_file = "end of file on ";

    void at_end(
        std::string left_file_name,
        std::istream& left,
        std::string right_file_name,
        std::istream& right,
        std::ostream& out
    ) {
        if (left.eof() && right.eof())
            return;

        out << end_of_file
            << (left.eof() ? left_file_name : right_file_name)
            << '\n';
    }
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

            if (left.eof() || right.eof()) {
                at_end(left_file_name, left, right_file_name, right, out);
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