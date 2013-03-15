// entab - convert runs of blanks into tabs
// entab copies its input to its output, replacing strings of blanks by tabs so
// that the output is visually the same as the input, but contains fewer
// characters. Tabs stops are assumed to be every four characters (1, 5, 9, ...)
// so that each sequence of one to four blanks ending on a tab stop is replaced
// by a tab character

#include <iostream>

bool tabstop(const int column)
{
  return column % 4 == 0;
} // tabstop

int main()
{
  const int BLANK = ' ';
  const int TAB = '\t';
  const int NEWLINE = '\n';

  int column = 0;
  int c = 0;

  while(c != -1)
  {
    int newcolumn = column;
    while((c = std::cin.get()) == BLANK)
    {
      ++newcolumn;
      if(tabstop(newcolumn))
      {
        std::cout.put(TAB);
        column = newcolumn;
      }
    }

    for( ; column < newcolumn; ++column)
      std::cout.put(BLANK);

    if(c != -1)
    {
      std::cout.put(c);
      column = (c == NEWLINE) ? 0 : column+1;
    }
  } // while
} // main
