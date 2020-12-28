#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include "../../lib/getline.hpp"

int main() {
  auto lines = std::vector<std::string> { };

  while (std::cin)
    lines.emplace_back(stiX::getline(std::cin));

  std::sort(lines.begin(), lines.end());

  std::copy(
    lines.begin(),
    lines.end(),
    std::ostream_iterator<std::string>(std::cout, "\n")
  );
}