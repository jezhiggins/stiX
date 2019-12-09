#include "include.hpp"

namespace stiX {
    std::string getline(std::istream &input) {
        std::string line;
        std::getline(input, line);
        return line;
    } // getline
}
