#ifndef STICPP_TAB_STOPS_H
#define STICPP_TAB_STOPS_H

namespace stiX {
    inline const size_t tabSize = 4;

    inline size_t next_tab_stop(size_t position) {
        const auto currentTab = (position / tabSize);
        const auto nextTab = currentTab + 1;
        const auto nextStop = nextTab * tabSize;
        return nextStop;
    }

    inline size_t distance_to_next_tab_stop(size_t position) {
        return next_tab_stop(position) - position;
    }

    inline bool is_tab_stop(size_t position) {
        return (position % tabSize) == 0;
    }
}

#endif //STICPP_TAB_STOPS_H
