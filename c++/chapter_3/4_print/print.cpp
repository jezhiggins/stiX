#include "print.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

void header(
  std::string const& filename,
  size_t page_number,
  std::ostream& output
);
void footer(std::ostream& output);
std::string getline(std::istream& input);
size_t count_header_lines();
size_t count_footer_lines();

auto const header_lines = count_header_lines();
auto const footer_lines = count_footer_lines();

class page_printer
{
public:
  page_printer(
    std::string const& filename,
    std::istream& input,
    std::ostream& output,
    size_t page_length
  ) :
      filename_(filename),
      input_(input),
      output_(output),
      page_length_(page_length),
      page_count_(0),
      line_count_(0),
      last_line_(page_length - footer_lines) {
  } // page_printer

  page_printer() = delete;
  page_printer(page_printer const&) = delete;

  size_t print() {
    while(input_available()) {
      if (at_page_start())
        print_header();

      print_line();

      if (at_last_line())
        print_footer();
    } // while ...

    pad_to_page_end();

    return page_count_;
  } // print
private:
  bool input_available() const { return input_ && !input_.eof(); }
  bool at_page_start() const { return line_count_ == 0; }
  bool at_last_line() const { return line_count_ == last_line_; }

  void print_header() {
    header(filename_, ++page_count_, output_);
    line_count_ += header_lines;
  } // print_header

  void print_line() {
    std::string line = stiX::getline(input_);
    output_ << line << '\n';
    ++line_count_;
  } // print_line

  void print_blank_line() {
    output_ << '\n';
    ++line_count_;
  } // print_blank_line

  void print_footer() {
    footer(output_);
    line_count_ = 0;
  } // print_footer

  void pad_to_page_end() {
    if (at_page_start())
      return;

    while (!at_last_line())
      print_blank_line();
    print_footer();
  } // pad_to_page_end

  std::string const& filename_;
  std::istream& input_;
  std::ostream& output_;
  size_t const page_length_;
  size_t page_count_;
  size_t line_count_;
  size_t const last_line_;
};

namespace stiX {
  int print(
    std::string const& filename,
    std::istream& input,
    std::ostream& output,
    size_t page_length
  ) {
    auto printer = page_printer(
        filename,
        input,
        output,
        page_length
    );

    return printer.print();
  } // print
} // namespace stiX

void header(
  std::string const& filename,
  size_t page_number,
  std::ostream& output
) {
  output << "\n\n"
         << filename
         << " Page " << page_number << '\n'
         << "\n\n";
} // header

void footer(std::ostream& output) {
  output << "\n\n";
} // footer

size_t countLines(std::string const& s) {
  return std::count(s.cbegin(), s.cend(), '\n');
} // countLines

size_t count_header_lines() {
  std::stringstream ss;
  header("", 0, ss);
  return countLines(ss.str());
} // count_header_lines

size_t count_footer_lines() {
  std::stringstream ss;
  footer(ss);
  return countLines(ss.str());
} // count_footer_lines

