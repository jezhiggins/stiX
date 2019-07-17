//
// Created by Jez Higgins on 2019-07-17.
//

#include "charcount.h"
#include <iostream>

namespace stiX {
    size_t charcount(std::istream &in) {
        return std::distance(
                std::istreambuf_iterator<char>(in),
                std::istreambuf_iterator<char>()
        );
    }
}