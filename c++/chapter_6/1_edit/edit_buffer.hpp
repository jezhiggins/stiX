#ifndef STICPP_EDIT_BUFFER_HPP
#define STICPP_EDIT_BUFFER_HPP

#include <string>
#include <vector>

namespace stiX {
  class edit_buffer {
  public:
    void insert(size_t index, std::string_view line);

    size_t size() const { return lines_.size(); }

  private:
    std::vector<std::string> lines_;
  };
}

#endif //STICPP_EDIT_BUFFER_HPP
