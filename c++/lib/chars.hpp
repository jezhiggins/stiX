#ifndef STICPP_CHARTESTS_H
#define STICPP_CHARTESTS_H

#include <string_view>

namespace stiX {
    inline bool isspace(char c) { return c == ' '; }
    inline bool istab(char c) { return c == '\t'; }
    inline bool isnewline(char c) { return c == '\n'; }
    inline bool isbackspace(char c) { return c == '\b'; }

    bool isalnum(char c);
    bool iswhitespace(char c);

    bool isalnum(std::string_view s);
}

#endif //STICPP_CHARTESTS_H
