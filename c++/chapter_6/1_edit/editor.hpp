#ifndef STICPP_EDITOR_H
#define STICPP_EDITOR_H

#include "edit_buffer.hpp"

namespace stiX {

  class editor {
  public:
    void insert_before(size_t index, std::string_view line);

    size_t dot() const { return dot_; }
    size_t last() const { return buffer_.size(); }

  private:
    size_t dot_ = 0;
    edit_buffer buffer_;
  };

}
#endif //STICPP_EDITOR_H
