#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace boost::lambda;

template<typename T>
class buf_iterator : public std::istreambuf_iterator<T>
{
public:
  buf_iterator(std::istream& s) :
      std::istreambuf_iterator<T>(s) { }
  buf_iterator() : 
      std::istreambuf_iterator<T>() { }

  bool operator==(const buf_iterator<T>& rhs)
  {
    return std::istreambuf_iterator<T>::operator==(rhs);
  }

  const T& operator*() const
  {
    c_ = std::istreambuf_iterator<T>::operator*();
    return c_;
  } 

  buf_iterator<T>& operator++()
  {
    std::istreambuf_iterator<T>::operator++();
    return *this;
  } 

private:
  mutable char c_;
};

int main()
{
  int count = 0;
  std::for_each(buf_iterator<char>(std::cin),
                buf_iterator<char>(),
                count += (_1 == '\n') );
  std::cout << count << std::endl;
} // main
