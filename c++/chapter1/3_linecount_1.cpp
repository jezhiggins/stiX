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
