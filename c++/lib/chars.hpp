#ifndef STICPP_CHARTESTS_H
#define STICPP_CHARTESTS_H

namespace stiX {
    inline bool isspace(char c) { return c == ' '; }
    inline bool istab(char c) { return c == '\t'; }
    inline bool isnewline(char c) { return c == '\n'; }
    inline bool isbackspace(char c) { return c == '\b'; }

    bool isalnum(char c);
}

#endif //STICPP_CHARTESTS_H
