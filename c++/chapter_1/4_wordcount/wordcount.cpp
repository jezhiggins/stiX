#include "wordcount.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace stiX {
    size_t wordcount(std::istream& in) {
        return std::distance(
                std::istream_iterator<std::string>(in),
                std::istream_iterator<std::string>()
        );
    }
}
