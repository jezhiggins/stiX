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
    void flush_if_wraps(std::string const& line);
    void flush();

    void line_print(std::string_view line);
    void blank_line();
    void page_end();

    void nf_no_fill();
    void fi_fill_on();
    void set_fill_mode(bool on);

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