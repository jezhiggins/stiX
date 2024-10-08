#ifndef STICPP_FORMATTER_HPP
#define STICPP_FORMATTER_HPP

#include <iosfwd>
#include <string>
#include <limits>
#include <optional>
#include <functional>
#include "filler.hpp"

namespace stiX {
  using page_callback = std::function<void()>;

  class screen_formatter {
  public:
    screen_formatter(std::istream& in, std::ostream& out);
    screen_formatter(
      std::istream& in,
      std::ostream& out,
      size_t page_width,
      size_t page_length,
      page_callback callback);

    void format();

    screen_formatter() = delete;
    screen_formatter(screen_formatter const&) = delete;
    screen_formatter(screen_formatter&&) = delete;

  private:
    void handle_command(std::string const& line);
    void handle_text(std::string line);
    void output_line(std::string const& line);
    void leading_blanks(std::string& line);
    void apply_styles(std::string& line);
    using style_fn = std::string(std::string const&);
    static void apply_style(std::string& line, size_t& control_var, style_fn styler);
    void apply_underlining(std::string& line);
    void apply_bold(std::string& line);
    void apply_italic(std::string& line);
    void apply_strikethrough(std::string& line);
    void buffer_line(std::string const& line);
    void fill_buffer();
    void fill_and_flush();
    void flush();

    void print_line(std::string const& line);
    std::string leading_indent();
    void line_spacing();
    void put_line(std::string const& line);
    void line_feed();
    void line_feed(size_t count);
    void print_centred_line(std::string const& line);
    void print_blank_line();
    void page_end();
    void print_header();
    void print_footer();
    void print_title(std::string const& title, size_t margin_before, size_t margin_after);

    enum class value_type {
      Absolute,
      Offset
    };

    struct command_parameter {
      int value;
      value_type type;
    };
    static command_parameter parse_value(
      std::string const& line,
      int def_value) ;
    static std::string string_param(
      std::string const& line) ;

    void nf_no_fill();
    void fi_fill_on();
    void set_fill_mode(bool on);
    void set_output_mode();
    void vertical_space(command_parameter const& param);
    void page_break(command_parameter const& param);
    void set_right_margin(command_parameter const& param);
    void set_page_length(command_parameter const& param);
    void set_line_space(command_parameter const& param);
    void set_centre(command_parameter const& param);
    void set_underline(command_parameter const& param);
    void set_bold(command_parameter const& param);
    void set_italic(command_parameter const& param);
    void set_stikethrough(command_parameter const& param);
    void set_indent(command_parameter const& param);
    void set_next_indent(command_parameter const& param);
    void clear_next_indent();
    static void set_variable(size_t& var,
                      command_parameter update,
                      size_t minimum = 0,
                      size_t maximum = std::numeric_limits<size_t>::max());

    void set_header(std::string const& header);
    void set_footer(std::string const& footer);

    size_t fillable_width() const;
    size_t indent() const;
    size_t lines_remaining() const;
    size_t bottom_margin() const;

    std::istream& in_;
    std::ostream& out_;

    size_t right_margin_;
    size_t page_length_;

    page_callback page_callback_;

    size_t line_space_;
    size_t centering_;
    size_t underline_;
    size_t bold_;
    size_t italic_;
    size_t strikethrough_;

    std::string header_;
    std::string footer_;

    bool fill_;
    filler filler_;
    size_t indent_;
    std::optional<size_t> next_indent_;

    size_t current_line_;
    size_t current_page_;
    std::string buffer_;

    using output_fn = void(screen_formatter::*)(std::string const&);
    output_fn output_mode_;
  };
}

#endif