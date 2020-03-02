#ifndef STICPP_INCLUDE_HPP
#define STICPP_INCLUDE_HPP

#include <iostream>
#include <string>
#include <fstream>

namespace stiX {
    std::string getline(std::istream& input);
    bool is_include(const std::string& line);
    std::string extract_included(const std::string& include_line);
    std::ifstream file_opener(const std::string& filename);

    template<typename IncludeOpener>
    decltype(auto) open_include(
        const std::string& include_line,
        std::ostream& output,
        IncludeOpener openFn
    ) {
        auto name = extract_included(include_line);
        auto included = openFn(name);
        if (!included) {
            output
                << "ERROR: Could not include \""
                << name
                << "\"\n";
        }
        return included;
    }

    template<typename IncludeOpener = typeof(file_opener)>
    bool expand_include(
        std::istream& input,
        std::ostream& output,
        IncludeOpener openFn = file_opener
    ) {
        bool good = true;
        while(input && good) {
            auto line = getline(input);

            if (!is_include(line)) {
                output << line
                       << (!input.eof() ? "\n" : "");
            } else {
                auto included = open_include(
                    line,
                    output,
                    openFn
                );
                if (!included)
                    return false;

                good = expand_include(
                    included,
                    output,
                    openFn
                );
            }
        } // while
        return good;
    } // include
}
#endif //STICPP_INCLUDE_HPP
