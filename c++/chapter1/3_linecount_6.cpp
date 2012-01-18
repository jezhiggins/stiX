#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
  int count = std::count_if(std::istreambuf_iterator<char>(std::cin),
			    std::istreambuf_iterator<char>(),
			    [](const char& i)->bool { return (i == '\n'); } );
  std::cout << count << std::endl;
} // main
