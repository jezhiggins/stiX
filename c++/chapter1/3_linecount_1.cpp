// linecount - count lines in the input
// linecount counts the lines in its input and writes the total as a line of
// text to the output.

#include <iostream>

const char NEWLINE = '\n';

int main()
{
  int nl = 0;
  char c = 0;
  while(std::cin.get(c))
    if(c == NEWLINE)
      ++nl;
  std::cout << nl << std::endl;
} // main
