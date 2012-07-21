// charcount - count characters in input
// charcount count the characters in its input and writes the total as a single
// line of text to the output. Since each line of text is internally delimited
// by a NEWLINE character, the total is the number of lines plus the number
// of characters within each line.

#include <iostream>

int main()
{
  int nc = 0;
  char c = 0;
  while(std::cin.get(c))
    ++nc;
  std::cout << nc << std::endl;
} // main
