#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
  int count = 0;
  std::for_each(std::istreambuf_iterator<char>(std::cin),
                std::istreambuf_iterator<char>(),
                [&count](const char& i) { count += (i == '\n'); } );
  std::cout << count << std::endl;
} // main
