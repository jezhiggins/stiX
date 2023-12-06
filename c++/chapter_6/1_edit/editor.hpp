#ifndef STICPP_EDITOR_H
#define STICPP_EDITOR_H

#include <iosfwd>
#include "edit_buffer.hpp"

namespace stiX {
  class editor {
  public:
    void process(std::istream& in, std::ostream& out);

  private:
    edit_buffer buffer_;
    std::string filename_;
  };
}

#endif //STICPP_EDITOR_H
