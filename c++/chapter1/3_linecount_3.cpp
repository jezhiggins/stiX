#include <iostream>
#include <iterator>
#include <algorithm>

class oh_look_a_new_line
{
public:
  oh_look_a_new_line(int& count) : count_(count) { }

  void operator()(const char& ch) 
  { 
    count_ += (ch == '\n'); 
  } // operator()

private:
  int& count_;
}; // class ...

int main()
{
  int count = 0;
  std::for_each(std::istreambuf_iterator<char>(std::cin),
                std::istreambuf_iterator<char>(),
                oh_look_a_new_line(count));
  std::cout << count << std::endl;
} // main
