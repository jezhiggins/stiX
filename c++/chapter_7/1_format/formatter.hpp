#ifndef STICPP_FORMATTER_HPP
#define STICPP_FORMATTER_HPP

#include <iosfwd>
#include <string>

namespace stiX {
  class screen_formatter {
  public:
    screen_formatter(std::istream& in, std::ostream &out);

    void format();

    screen_formatter() = delete;
    screen_formatter(screen_formatter const&) = delete;
    screen_formatter(screen_formatter&&) = delete;

  private:
    void flush_if_wraps();
    void flush();
    
    void page_end();

    std::istream& in_;
    std::ostream& out_;

    size_t line_;
    size_t max_width_;
    size_t max_lines_;
    std::string buffer_;
  };
}

#endif