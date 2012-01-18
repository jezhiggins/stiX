#include <iostream>

int main()
{
  int nc = 0;
  char c = 0;
  while(std::cin.get(c))
    ++nc;
  std::cout << nc << std::endl;
} // main
