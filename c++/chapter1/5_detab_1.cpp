// detab - converts tabs to blanks
// detab copies its input to its output, expanding horizontal tabs to blanks
// along the way, so that the output is visually the same as the input, but
// contains no tab characters. Tab stops are assumed to be set every four
// columns (i.e. 1, 5, 9, ...), so that each tab character is replace by from
// one to four blanks

#include <iostream>

bool tabstop(const int column)
{
  return column % 4 == 0;
} // tabstop

int main()
{
  const char NEWLINE = '\n';
  const char TAB = '\t';
  const char BLANK = ' ';

  int column = 0;
  char c = 0;

  while(std::cin.get(c))
    switch(c)
    {
    case TAB:
      while(!tabstop(column))
      {
        std::cout << BLANK;
        ++column;
      }
      break;
    case NEWLINE:
      std::cout << NEWLINE;
      column = 0;
      break;
    default:
      std::cout << c;
      ++column;
    }
} // main
