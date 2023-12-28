#ifndef STICPP_EDITOR_H
#define STICPP_EDITOR_H

#include <iosfwd>
#include "edit_buffer.hpp"

namespace stiX {
  class editor {
  public:
    void load(std::string_view filename, std::ostream& out);
    void process(std::istream& in, std::ostream& out);

  private:
    void handle_command(std::string_view line, std::istream& in, std::ostream& out);

    edit_buffer buffer_;
    std::string filename_;
  };
}

#endif //STICPP_EDITOR_H
