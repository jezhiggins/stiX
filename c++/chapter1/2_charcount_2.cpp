#include <iostream>

int main()
{
  int nc = 0;
  while(std::cin.get() != -1)
    ++nc;
  std::cout << nc << std::endl;
} // main
