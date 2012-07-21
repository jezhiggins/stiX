#include <iostream>

int main()
{
  int c = 0;
  while((c = std::cin.get()) != -1)
    std::cout.put(c);
  return 0;
} // main
