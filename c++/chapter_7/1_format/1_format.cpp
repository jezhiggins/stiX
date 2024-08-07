//
// `format`   product formatted output
//
// `format`
//
// `format` reads its input a line at a time and writes a neatly formatted version
// of the input text to the output, with page headers and footers and with output
// lines filled to a uniform right margin. Input text lines may have interspersed
// among them command lines that alter the default mode of the formatting. A
// command line consists of a leading period, followed by a two letter code,
// possibly with optional arguments following the first sequence of blanks and tabs.
//
// Certain commands cause a "break" in the processing of input text lines, ie any
// partially filled line is output and a new line is begun. In the following command
// summary, the letter n stands for an optional numeric argument. If a numeric
// argument is preceded by a + or - the current value is changed by this amount;
// otherwise the argument represents the new value. If no argument is given, the
// default value is used.
//
// command    break?    default     function
//
//  .bp n      yes       n=+1       begin page numbered n
//  .br                  yes        cause break
//  .ce n      yes       n=1        centre next n lines
//  .fi        yes                  start filling
//  .fo str    no        empty      footer title
//  .he str    no        empty      header title
//  .in n      no        n=0        indent n spaces
//  .ls n      no        n=1        line spacing is n
//  .nf        yes                  stop filling
//  .pl n      no        n=66       set page length to n
//  .rm n      no        n=60       set right margin to n
//  .sp n      yes       n=1        space down n lines or to bottom of page
//  .ti n      yes       n=0        temporary indent of n
//  .ul n      no        n=1        underline words from next n lines
//
// A blank input line causes a break and is passed to the output unchanged.
// Similarly, an input line that begins with blanks causes a break and is written
// to the output with the leading blanks preserved. Thus a document formatting in
// the conventional manner by hand will retain its original paragraph breaks and
// indentation.
//
// My extensions
//  .bo n      yes       n=1        bold words from next n lines
//  .it n      yes       n=1        italicise words from the next n lines
//  .st n      yes       n=1        strikethrough words from the next n lines
//

#include "format.hpp"
#include "../../lib/arguments.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <tuple>
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace {
  std::tuple<size_t, size_t> console_size() {
#ifdef __linux__
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    auto width = std::min<size_t>(w.ws_col, 200);
    auto length = std::min<size_t>(w.ws_row, 66);
    width = width != 0 ? width : 60;
    length = length != 0 ? length : 66;
    return { width, length };
#else
    return { 60, 66 };
#endif
  }

  void wait_for_keypress() {
    getchar();
  }

  void format_file(std::string const& filename) {
    auto file = std::ifstream(filename);
    if (!file)
      return;

    auto [width, length] = console_size();
    stiX::format(file, std::cout, width, length, wait_for_keypress);
  }

  void format_files(std::vector<std::string> const& filenames) {
    std::ranges::for_each(filenames, format_file);
  }

  void format_cin() {
    auto [width, length] = console_size();
    stiX::format(std::cin, std::cout, width, length);
  }
}

int main(int argc, char const* argv[]) {
  auto filenames = stiX::make_arguments(argc, argv);

  if (filenames.empty())
    format_cin();
  else
    format_files(filenames);
}
