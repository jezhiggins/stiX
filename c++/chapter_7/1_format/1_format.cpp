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
//x .br                  yes        cause break
//x .ce n      yes       n=1        centre next n lines
//x .fi        yes                  start filling
//  .fo str    no        empty      footer title
//  .he str    no        empty      header title
//  .in n      no        n=0        indent n spaces
//x .ls n      no        n=1        line spacing is n
//x .nf        yes                  stop filling
//x .pl n      no        n=66       set page length to n
//x .rm n      no        n=60       set right margin to n
//x .sp n      yes       n=1        space down n lines or to bottom of page
//  .ti n      yes       n=0        temporary indent of n
// x.ul n      no        n=1        underline words from next n lines
//
// A blank input line causes a break and is passed to the output unchanged.
// Similarly, an input line that begins with blanks causes a break and is written
// to the output with the leading blanks preserved. Thus a document formatting in
// the conventional manner by hand will retain its original paragraph breaks and
// indentation.
//

#include "format.hpp"
#include <iostream>
#include <algorithm>
#ifdef __linux__
#include <sys/ioctl.h>
#include <unistd.h>
#endif

int main() {
#ifdef __linux__
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  
  auto width = std::min<size_t>(w.ws_col, 200);
  auto length = std::min<size_t>(w.ws_row, 66);
  width = width != 0 ? width : 60;
  length = length != 0 ? length : 66;

  stiX::format(std::cin, std::cout, width, length);
#else
  stiX::format(std::cin, std::cout);
#endif
}
