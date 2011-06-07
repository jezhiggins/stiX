#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

using namespace boost::lambda;

int main()
{
  std::for_each(std::istreambuf_iterator<char>(std::cin),
                std::istreambuf_iterator<char>(),
		std::cout << _1);

  std::cout << std::endl;
} // main
