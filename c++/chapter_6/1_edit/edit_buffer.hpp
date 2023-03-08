#ifndef STICPP_EDIT_BUFFER_H
#define STICPP_EDIT_BUFFER_H

#include "line_buffer.hpp"
#include "lines.hpp"

namespace stiX {

  class edit_buffer : public lines {
  public:
    void insert_before(size_t index, std::string_view line);

    std::string_view operator[](size_t index) const;

    size_t dot() const { return dot_; }
    size_t last() const { return buffer_.size(); }

    bool empty() const { return buffer_.size() == 0; }

  private:
    size_t dot_ = 0;
    line_buffer buffer_;
  };

}
#endif //STICPP_EDIT_BUFFER_H
