#ifndef STICPP_EDITOR_H
#define STICPP_EDITOR_H

#include <iosfwd>

namespace stiX {
  class editor {
  public:
    void process(std::istream& in, std::ostream& out);
  };
}

#endif //STICPP_EDITOR_H
