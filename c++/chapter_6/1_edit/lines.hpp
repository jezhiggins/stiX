#ifndef STICPP_LINES_H
#define STICPP_LINES_H

#include <string_view>

namespace stiX {
  class lines {
  public:
    virtual std::string_view line_at(size_t index) const = 0;

    virtual size_t dot() const = 0;
    virtual size_t last() const = 0;

  protected:
    ~lines() = default;
  };

  class lines_modifier : public lines {
  public:
    virtual void insert(size_t index, std::string_view line) = 0;
    virtual void set_at(size_t index, std::string_view line) = 0;
    virtual void remove_at(size_t index) = 0;
    virtual void swap(size_t lindex, size_t rindex) = 0;

    virtual void set_dot(size_t new_dot) = 0;

    virtual bool empty() const = 0;
  };
}
#endif //STICPP_LINES_H
