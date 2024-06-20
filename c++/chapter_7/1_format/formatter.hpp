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
    void handle_command(std::string const& line);
    void handle_text(std::string const& line);
    void flush_if_wraps();
    void flush();

    void line(std::string_view line);
    void page_end();

    std::istream& in_;
    std::ostream& out_;

    size_t line_;
    size_t max_width_;
    size_t max_lines_;
    std::string buffer_;

    bool fill_;
  };
}

#endif