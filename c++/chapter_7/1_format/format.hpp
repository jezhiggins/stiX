#ifndef STICPP_FORMAT_HPP
#define STICPP_FORMAT_HPP

#include <iosfwd>
#include <functional>

namespace stiX {
  using page_callback = std::function<void()>;

  void format(std::istream& in, std::ostream& out);
  void format(
    std::istream& in,
    std::ostream& out,
    size_t page_width,
    size_t page_length);
  void format(
    std::istream& in,
    std::ostream& out,
    size_t page_width,
    size_t page_length,
    page_callback callback);
}

#endif