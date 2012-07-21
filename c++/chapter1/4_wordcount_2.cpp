#include <iostream>
#include <iterator>
#include <algorithm>

class wordcounter
{
  public:
    wordcounter() :
      inword_(false)
    {
    } // wordcounter

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
    static const char TAB = '\t';
    static const char BLANK = ' ';
}; // class wordcounter

int main()
{
  int count = std::count_if(std::istreambuf_iterator<char>(std::cin),
                            std::istreambuf_iterator<char>(),
                            wordcounter());
  std::cout << count << std::endl;
} // main
