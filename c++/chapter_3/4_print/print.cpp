#include "print.hpp"
#include "../../lib/getline.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

void header(
  std::string const& filename,
  size_t pageNumber,
  std::ostream& output
);
void footer(std::ostream& output);
std::string getline(std::istream& input);
size_t countHeaderLines();
size_t countFooterLines();

size_t const headerLines = countHeaderLines();
size_t const footerLines = countFooterLines();

class page_printer
{
public:
  page_printer(
    std::string const& filename,
    std::istream& input,
    std::ostream& output,
    size_t pageLength
  ) :
    filename_(filename),
    input_(input),
    output_(output),
    pageLength_(pageLength),
    pageCount_(0),
    lineCount_(0),
    lastLine_(pageLength - footerLines) {
  } // page_printer

  page_printer() = delete;
  page_printer(page_printer const&) = delete;

  size_t print() {
    while(input_available()) {
      if (at_page_start())
        print_header();

      print_line();

      if (at_page_end())
        print_footer();
    } // while ...

    if (!at_page_start()) {
      while (!at_page_end())
        print_blank_line();
      print_footer();
    }

    return pageCount_;
  } // print
private:
  bool input_available() const { return input_ && !input_.eof(); }
  bool at_page_start() const { return lineCount_ == 0; }
  bool at_page_end() const { return lineCount_ == lastLine_; }

  void print_header() {
    header(filename_, ++pageCount_, output_);
    lineCount_ += headerLines;
  } // print_header

  void print_line() {
    std::string line = stiX::getline(input_);
    output_ << line << '\n';
    ++lineCount_;
  } // print_line

  void print_blank_line() {
    output_ << '\n';
    ++lineCount_;
  } // print_blank_line

  void print_footer() {
    footer(output_);
    lineCount_ = 0;
  } // print_footer

  std::string const& filename_;
  std::istream& input_;
  std::ostream& output_;
  size_t const pageLength_;
  size_t pageCount_;
  size_t lineCount_;
  size_t const lastLine_;
};

namespace stiX {
  int print(
    std::string const& filename,
    std::istream& input,
    std::ostream& output,
    size_t pageLength
  ) {
    auto printer = page_printer(
      filename,
      input,
      output,
      pageLength
    );

    return printer.print();
  } // print
} // namespace stiX

void header(
  std::string const& filename,
  size_t pageNumber,
  std::ostream& output
) {
  output << "\n\n"
         << filename
         << " Page " << pageNumber << '\n'
         << "\n\n";
} // header

void footer(std::ostream& output) {
  output << "\n\n";
} // footer

size_t countLines(std::string const& s) {
  return std::count(s.cbegin(), s.cend(), '\n');
} // countLines

size_t countHeaderLines() {
  std::stringstream ss;
  header("", 0, ss);
  return countLines(ss.str());
} // countHeaderLines

size_t countFooterLines() {
  std::stringstream ss;
  footer(ss);
  return countLines(ss.str());
} // countFooterLines

