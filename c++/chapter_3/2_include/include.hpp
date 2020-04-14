#ifndef STICPP_INCLUDE_HPP
#define STICPP_INCLUDE_HPP

#include <iostream>
#include <string>
#include <fstream>

namespace stiX {
    std::string getline(std::istream& input);
    bool is_include(std::string const& line);
    std::string extract_included(std::string const& include_line);
    std::ifstream file_opener(std::string const& filename);

    template<typename IncludeOpener>
    decltype(auto) open_include(
        std::string const& include_line,
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

    template<typename IncludeOpener = decltype(file_opener)>
    void expand_include(
        std::istream& input,
        std::ostream& output,
        IncludeOpener openFn = file_opener
    ) {
        while(input) {
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
                    return;

                expand_include(
                    included,
                    output,
                    openFn
                );
            }
        } // while
    } // include
}
#endif //STICPP_INCLUDE_HPP
