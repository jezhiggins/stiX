#ifndef STICPP_EDIT_BUFFER_H
#define STICPP_EDIT_BUFFER_H

#include "line_buffer.hpp"
#include "lines.hpp"

namespace stiX {
  class edit_buffer : public lines_modifier {
  public:
    void insert(size_t index, std::string_view line) override;
    void set_at(size_t index, std::string_view line) override;
    void remove_at(size_t index) override;
    void swap(size_t lindex, size_t rindex) override;

    std::string_view line_at(size_t index) const override;

    void set_dot(size_t new_dot) override { dot_ = new_dot; }
    size_t dot() const override { return dot_; }
    size_t last() const override { return buffer_.size(); }

    bool empty() const override { return buffer_.size() == 0; }

  private:
    size_t dot_ = 0;
    line_buffer buffer_;
  };

}
#endif //STICPP_EDIT_BUFFER_H
