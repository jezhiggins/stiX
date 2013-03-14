// detab - converts tabs to blanks
// detab copies its input to its output, expanding horizontal tabs to blanks
// along the way, so that the output is visually the same as the input, but
// contains no tab characters. Tab stops are assumed to be set every four
// columns (i.e. 1, 5, 9, ...), so that each tab character is replace by from
// one to four blanks

#include <iostream>
#include <iterator>
#include <algorithm>

class detabber {
public:
  detabber() :
    column_(0) 
  {
  }

  void operator()(const char& c)
  {
    if(c == TAB)
      while(!at_tabstop())
	output(BLANK);
    else
      output(c);
  } // operator()

private:
  void output(const char& c) 
  {
    std::cout << c;
    column_ = (c != NEWLINE) ? column_+1 : 0;
  } // output

  bool at_tabstop()
  {
    return column_ % 4 == 0;
  } // at_tabstop

  int column_ = 0;

  const char NEWLINE = '\n';
  const char TAB = '\t';
  const char BLANK = ' ';
}; // class detabber

int main()
{
  std::for_each(std::istreambuf_iterator<char>(std::cin),
		std::istreambuf_iterator<char>(),
		detabber());
} // main
