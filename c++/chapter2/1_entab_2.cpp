#include <iostream>
#include <iterator>
#include <algorithm>

class entabber {
public:
  entabber() :
    column_(0),
    newcol_(0)
  {
  }

  void operator()(const char& c)
  {
    if(c != BLANK) 
    {
      while(column_ != newcol_)
	output(BLANK);
	
      output(c);

      newcol_ = column_;
      return;
    } // if ...

    ++newcol_;
    if(at_tabstop(newcol_))
    {
      output(TAB);
      column_ = newcol_;
    }
  } // operator()

private:
  void output(const char& c) 
  {
    std::cout << c;
    column_ = (c != NEWLINE) ? column_+1 : 0;
  } // output

  bool at_tabstop(int col) const
  {
    return col % 4 == 0;
  } // at_tabstop

  int column_;
  int newcol_;

  const char NEWLINE = '\n';
  const char TAB = '\t';
  const char BLANK = ' ';
}; // class detabber

int main()
{
  std::for_each(std::istreambuf_iterator<char>(std::cin),
		std::istreambuf_iterator<char>(),
		entabber());
} // main
