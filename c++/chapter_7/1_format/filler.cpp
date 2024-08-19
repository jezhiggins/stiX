#include "filler.hpp"
#include <algorithm>
#include "format_functions.hpp"

namespace {
  class reverser {
  public:
    reverser(std::string &buffer, bool direction)
      : b(buffer), d(direction) {
      flip();
    }

    ~reverser() {
      flip();
    }

  private:
    void flip() {
      if (d)
        std::reverse(b.begin(), b.end());
    }

    std::string& b;
    bool d;
  };

  void fill_in_alt_directions(std::string& buffer, size_t width, bool direction) {
    auto r = reverser(buffer, direction);
    buffer = stiX::fill_line(buffer, width);
  }
}

std::string stiX::filler::operator()(std::string const& line_in, size_t width) {
  auto buffer = line_in;
  fill_in_alt_directions(buffer, width, direction_);
  direction_ = !direction_;
  return buffer;
}


