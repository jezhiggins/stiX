#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
  std::cout << std::distance(std::istreambuf_iterator<char>(std::cin),
                             std::istreambuf_iterator<char>())
            << std::endl;
} // main
