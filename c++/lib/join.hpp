#ifndef STICPP_JOIN_H
#define STICPP_JOIN_H

#include <algorithm>

namespace stiX {
  template<typename InputIt, typename OutputIt, typename Separator = std::string>
  OutputIt join(InputIt first, InputIt last, OutputIt dest, Separator sep = " ") {
    if (first != last)
      *dest++ = *first++;
    while (first != last) {
      *dest++ = sep;
      *dest++ = *first++;
    }
    return dest;
  }

  template<typename InputRange, typename OutputIt, typename Separator = std::string>
  OutputIt join(InputRange &&range, OutputIt out, Separator sep = " ") {
    return join(std::begin(range), std::end(range), out, sep);
  }
}
#endif //STICPP_JOIN_H
