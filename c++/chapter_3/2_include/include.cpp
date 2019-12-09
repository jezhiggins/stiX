#include "include.hpp"

namespace stiX {
    std::string getline(std::istream &input) {
        std::string line;
        std::getline(input, line);
        return line;
    } // getline

    std::ifstream file_opener(std::string filename) {
        return std::ifstream(filename);
    } // file_opener

    std::string extract_included(std::string include_line)
    {
        size_t first_quote = stiX::hash_include.size();
        size_t second_quote = include_line.find('"', first_quote+1);
        return include_line.substr(
            first_quote,
            second_quote - first_quote
        );
    }
}
