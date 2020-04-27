#ifndef STICPP_FILTER_HPP
#define STICPP_FILTER_HPP

#include <iostream>
#include <iterator>
#include <algorithm>

namespace stiX {
    template <typename Transformer, typename OutputType = std::string>
    void filter(
        std::istream &in,
        std::ostream &out,
        Transformer t
    ) {
      std::transform(
          std::istreambuf_iterator<char>(in),
          std::istreambuf_iterator<char>(),
          std::ostream_iterator<OutputType>(out),
          t
      );
    }

    template <typename Transformer>
    void raw_filter(
        std::istream &in,
        std::ostream &out,
        Transformer t
    ) {
      filter<Transformer, char>(
          in,
          out,
          t
      );
    }
}

#endif //STICPP_FILTER_HPP
