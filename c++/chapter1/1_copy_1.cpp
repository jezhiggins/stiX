// copy - copy input to output
// copy copies its input to its output unchanged. It is useful for copy from a
// terminal to a file, or even from a terminal to a terminal. It may be used for
// displaying the contents of a file, without interpretation or formating, by
// copying from a file to a terminal.

#include <iostream>

int main()
{
  int c = 0;
  while((c = std::cin.get()) != -1)
    std::cout.put(c);
  return 0;
} // main
