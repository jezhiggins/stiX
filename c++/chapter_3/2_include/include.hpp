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

    template<typename IncludeOpener = typeof(file_opener)>
    void expand_include(
        std::istream& input,
        std::ostream& output,
        IncludeOpener openFn = file_opener
    ) {
        while(input) {
            std::string line = getline(input);

            if (!is_include(line)) {
                output << line
                       << (!input.eof() ? "\n" : "");
            } else {
                auto name = extract_included(line);
                auto included = openFn(name);
                expand_include(included, output, openFn);
            }
        } // while
    } // include
}
#endif //STICPP_INCLUDE_HPP
