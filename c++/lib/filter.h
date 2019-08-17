#ifndef STICPP_FILTER_H
#define STICPP_FILTER_H

#include <iostream>
#include <iterator>
#include <algorithm>

namespace stiX {
    template <typename Transformer>
    void filter(
        std::istream &in,
        std::ostream &out,
        Transformer t
    ) {
      std::transform(
          std::istreambuf_iterator<char>(in),
          std::istreambuf_iterator<char>(),
          std::ostream_iterator<std::string>(out),
          t
      );
    }
}

#endif //STICPP_FILTER_H
