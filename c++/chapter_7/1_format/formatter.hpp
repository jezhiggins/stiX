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

    std::istream& in_;
    std::ostream& out_;
    size_t maxlen_;
    std::string buffer_;
  };
}

#endif