#ifndef STICPP_LINES_H
#define STICPP_LINES_H

#include <string_view>

namespace stiX {
  class lines {
  public:
    virtual size_t dot() const = 0;
    virtual size_t last() const = 0;

  protected:
    ~lines() { }
  };

}
#endif //STICPP_LINES_H
