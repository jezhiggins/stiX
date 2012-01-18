#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
  int count = std::distance(std::istreambuf_iterator<char>(std::cin),
                            std::istreambuf_iterator<char>());
  std::cout << count << std::endl;
} // main
