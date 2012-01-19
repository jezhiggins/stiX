#include <iostream>

const char NEWLINE = '\n';
const char TAB = '\t';
const char BLANK = ' ';

int main()
{
  int nw = 0;
  char c = 0;
  bool inword = false;

  while(std::cin.get(c))
    if((c == BLANK) || (c == NEWLINE) || (c == TAB))
      inword = false;
    else if(!inword)
    {
      inword = true;
      ++nw;
    }

  std::cout << nw << std::endl;
  
  return 0;
} // main
