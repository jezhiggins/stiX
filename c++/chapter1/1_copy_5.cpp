#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
  std::for_each(std::istreambuf_iterator<char>(std::cin),
                std::istreambuf_iterator<char>(),
		[](const char& i) { std::cout << i; });

  std::cout << std::endl;
} // main
