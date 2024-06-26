#ifndef STICPP_FORMATTER_HPP
#define STICPP_FORMATTER_HPP

#include <iosfwd>
#include <string>
#include <limits>

namespace stiX {
  class screen_formatter {
  public:
    screen_formatter(std::istream& in, std::ostream& out);
    screen_formatter(std::istream& in, std::ostream& out, size_t page_width, size_t page_length);

    void format();

    screen_formatter() = delete;
    screen_formatter(screen_formatter const&) = delete;
    screen_formatter(screen_formatter&&) = delete;

  private:
    void handle_command(std::string const& line);
    void handle_text(std::string line);
    void line_buffer(std::string const& line);
    void fill_and_flush();
    void flush();

    void line_print(std::string_view line);
    void line_feed();
    void blank_line();
    void page_end();

    enum class value_type {
      Absolute,
      Offset
    };

    struct command_parameter {
      int value;
      value_type type;
    };
    command_parameter parse_value(
      std::string const& line,
      int def_value) const;

    void nf_no_fill();
    void fi_fill_on();
    void set_fill_mode(bool on);
    void vertical_space(command_parameter param);
    void set_right_margin(command_parameter param);
    void set_page_length(command_parameter param);
    void set_line_space(command_parameter param);
    void set_centre(command_parameter param);
    void set_underline(command_parameter param);
    void set_variable(size_t& var,
                      command_parameter update,
                      size_t minimum = 0,
                      size_t maximum = std::numeric_limits<size_t>::max());
    size_t lines_remaining() const;

    std::istream& in_;
    std::ostream& out_;

    size_t current_line_;
    size_t right_margin_;
    size_t page_length_;
    size_t line_space_;
    size_t centring_;
    size_t underline_;
    std::string buffer_;

    bool fill_;
  };
}

#endif