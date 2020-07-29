#include "./archive.hpp"

#include <iostream>

using namespace stiX;

void print_help();

void stiX::archive(std::vector<std::string> const& arguments) {
  if (arguments.size() < 2) {
    print_help();
    return;
  }

  auto const cmd = arguments[0];
  auto const archive = arguments[1];
  auto const files = std::vector<std::string>(arguments.begin() + 2, arguments.end());

  print_help();
} // archive

void print_help() {
  std::cout << R"c( archive -cmd aname [ file ... ]

 archive manages any number of member files in a single
 file, aname, with sufficient information that members
 may be selectively added, extracted, replaced, or
 deleted from the collection. -cmd is a code that
 determines the operation to be performed.

  -c  create a new archive with named members
  -d  deleted named members from archive
  -p  print named members on standard output
  -t  print table of archive contents
  -u  update named members or add at end
  -x  extract named members from archive
)c";
}