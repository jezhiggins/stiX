#include <iostream>
#include <iterator>
#include <algorithm>

class wordcount
{
  public:
    wordcount() :
	inword_(false)
    {
    } // wordcount

    bool operator()(const char& c) 
    {
      if((c == BLANK) || (c == NEWLINE) || (c == TAB))
	inword_ = false;
      else if(!inword_) 
      {
        inword_ = true;
	return true;
      }
      return false;
    } // operator

  private:
    bool inword_;
    static const char NEWLINE = '\n';
    static const char TAB = '\r';
    static const char BLANK = ' ';
};

int main()
{
  int count = std::count_if(std::istreambuf_iterator<char>(std::cin),
			    std::istreambuf_iterator<char>(),
                            wordcount());
  std::cout << count << std::endl;
} // main
