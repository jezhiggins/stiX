#include <iostream>
#include <string>

int main()
{
  int count = 0;
  std::string st;
  while(std::getline(std::cin, st))
    ++count;
  std::cout << count << std::endl;
} // main
