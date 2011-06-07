#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
  std::copy(std::istreambuf_iterator<char>(std::cin),
            std::istreambuf_iterator<char>(),
            std::ostream_iterator<char>(std::cout));
  return 0;
} // main
