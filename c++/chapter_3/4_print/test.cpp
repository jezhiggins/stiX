#include <sstream>
#include "../../testlib/testlib.h"
#include "print.hpp"

void testPrint(
    std::string input,
    size_t expectedPageCount
);

std::vector<std::string> split(std::string str) {
  std::vector<std::string> lines;
  std::string l;
  std::istringstream ss(str);
  while(std::getline(ss, l))
    lines.push_back(l);
  return lines;
}

const size_t pageLength = 4;

const std::string mary_had_a_little_lamb =
    R"c(Mary had a little lamb
Its fleece was white as snow
And everywhere that Mary went
The lamb was sure to go
)c";

TEST_CASE("Chapter 4 - print") {
  SECTION("no pages") {
    testPrint(
        std::string(),
        0
    );
  }

  SECTION("one page exactly") {
    testPrint(
        mary_had_a_little_lamb,
        1
    );
  }
}

void testPrint(
    std::string input,
    size_t expectedPageCount
) {
  std::istringstream inputs(input);
  std::ostringstream output;

  if (input.empty())
    inputs.setstate(std::ios_base::badbit);

  auto pageCount = stiX::print(
      "test",
      inputs,
      output,
      pageLength
  );

  REQUIRE(pageCount == expectedPageCount);

  auto pagedOutput = split(output.str());
  REQUIRE(pagedOutput.size() == expectedPageCount * pageLength);

  // verifyHeader(expectedPageCount, pagedOutput);
  // verifyPage(input, pagedOutput);
  // verifyFooter(expectedPageCount, pagedOutput);
}
