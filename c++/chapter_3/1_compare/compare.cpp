
#include "compare.hpp"
#include <iostream>

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
        size_t line = 1;
        while(true) {
            std::string leftline;
            std::string rightline;

            std::getline(left, leftline);
            std::getline(right, rightline);

            if (left.eof() && right.eof())
                return;
            if (left.eof() && !right.eof()) {
                out << end_of_file << left_file_name << '\n';
                return;
            }
            if (!left.eof() && right.eof()) {
                out << end_of_file << right_file_name << '\n';
                return;
            }

            if (leftline != rightline) {
                out << line << ':' << '\n'
                    << leftline << '\n'
                    << rightline << '\n';
            }

            ++line;
        }
    } // compare
}