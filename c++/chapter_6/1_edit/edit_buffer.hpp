#ifndef STICPP_EDIT_BUFFER_H
#define STICPP_EDIT_BUFFER_H

#include "line_buffer.hpp"

namespace stiX {
  class edit_buffer {
  public:
    void insert(size_t index, std::string_view line);
    void set_at(size_t index, std::string_view line) ;
    void remove_at(size_t index) ;
    void swap(size_t lindex, size_t rindex);

    std::string_view line_at(size_t index) const;
    bool mark(size_t index) const;

    void set_mark(size_t index);
    void clear_mark(size_t index);

    void set_dot(size_t new_dot) { dot_ = new_dot; }
    size_t dot() const { return dot_; }
    size_t last() const { return buffer_.size(); }

    void set_filename(std::string_view filename) {
      if (!filename.empty()) filename_ = filename;
    }
    std::string_view filename() const { return filename_; }

    bool empty() const { return buffer_.size() == 0; }

  private:
    size_t dot_ = 0;
    line_buffer buffer_;
    std::string filename_;
  };

}
#endif //STICPP_EDIT_BUFFER_H
