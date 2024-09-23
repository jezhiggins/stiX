#include <locale>
#include "chars.hpp"

const std::locale loc = std::locale();

bool stiX::isalnum(char c) { return std::isalnum(c, loc); }
bool stiX::iswhitespace(char c) { return std::isspace(c, loc); }