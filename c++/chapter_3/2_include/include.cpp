#include "include.hpp"
#include "../../lib/getline.hpp"

namespace stiX {
    const std::string hash_include("#include \"");

    bool is_include(std::string const& line) {
        return line.find(hash_include) != std::string::npos;
    }

   std::string extract_included(std::string const& include_line)
    {
        size_t first_quote = stiX::hash_include.size();
        size_t second_quote = include_line.find('"', first_quote+1);
        return include_line.substr(
            first_quote,
            second_quote - first_quote
        );
    }

    std::ifstream file_opener(std::string const& filename) {
        return std::ifstream(filename);
    } // file_opener
}
