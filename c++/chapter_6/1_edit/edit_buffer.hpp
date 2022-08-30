#ifndef STICPP_EDIT_BUFFER_H
#define STICPP_EDIT_BUFFER_H

#include "line_buffer.hpp"

namespace stiX {

  class edit_buffer {
  public:
    void insert_before(size_t index, std::string_view line);

    size_t dot() const { return dot_; }
    size_t last() const { return buffer_.size(); }

  private:
    size_t dot_ = 0;
    line_buffer buffer_;
  };

}
#endif //STICPP_EDIT_BUFFER_H
