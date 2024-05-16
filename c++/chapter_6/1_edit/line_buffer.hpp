#ifndef STICPP_LINE_BUFFER_HPP
#define STICPP_LINE_BUFFER_HPP

#include <string>
#include <vector>

namespace stiX {
  class line_buffer {
  public:
    void insert(size_t index, std::string_view line);
    void set(size_t index, std::string_view line);
    void remove(size_t index);
    void swap(size_t lindex, size_t rindex);

    std::string_view operator[](size_t index) const;

    size_t size() const { return lines_.size(); }

  private:
    std::vector<std::string> lines_;
  };
}

#endif //STICPP_LINE_BUFFER_HPP
