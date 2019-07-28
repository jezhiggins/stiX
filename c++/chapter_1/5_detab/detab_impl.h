#ifndef STICPP_DETAB_IMPL_H
#define STICPP_DETAB_IMPL_H

namespace stiX {
    inline const size_t tabSize = 8;

    size_t next_tab_stop(size_t position) {
        const auto currentTab = (position / tabSize);
        const auto nextTab = currentTab + 1;
        const auto nextStop = nextTab * tabSize;
        return nextStop;
    }
}

#endif //STICPP_DETAB_IMPL_H
