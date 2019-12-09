#ifndef STICPP_INCLUDE_HPP
#define STICPP_INCLUDE_HPP

#include <iostream>
#include <string>
#include <fstream>

namespace stiX {
    std::string getline(std::istream& input);
    std::string extract_included(std::string include_line);
    std::ifstream file_opener(std::string filename);

    const std::string hash_include("#include \"");

    template<typename IncludeOpener = typeof(file_opener)>
    void expand_include(
        std::istream& input,
        std::ostream& output,
        IncludeOpener openFn = file_opener
    ) {
        while(input) {
            std::string line = getline(input);

            if (line.find(hash_include) == std::string::npos) {
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
