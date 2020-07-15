#include <sstream>
#include "../../testlib/testlib.h"
#include "print.hpp"

void testPrint(
  std::string const& input,
  size_t expectedPageCount
);
void verifyHeader(
  size_t expectedPageCount,
  std::vector<std::string> const& lines
);
void verifyPage(
  std::string const& input,
  std::vector<std::string> const& lines
);
void verifyFooter(
  size_t expectedPageCount,
  std::vector<std::string> const& lines
);

std::vector<std::string> split(std::string const& str) {
  std::vector<std::string> lines;
  std::string l;
  std::istringstream ss(str);
  while(std::getline(ss, l))
    lines.push_back(l);
  return lines;
}

size_t const pageLength = 4;
size_t const totalPageLength = (pageLength + 5 + 2);
std::string const filename = "test";

std::string const mary_had_a_little_lamb =
  R"c(Mary had a little lamb
Its fleece was white as snow
And everywhere that Mary went
The lamb was sure to go)c";

TEST_CASE("Chapter 3 - print") {
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

  SECTION("one page, needs to pad") {
    testPrint(
      "One, two, buckle my shoe",
      1
    );
  }

  SECTION("two pages exactly") {
    testPrint(
      mary_had_a_little_lamb + "\n" + mary_had_a_little_lamb,
      2
    );
  }

  SECTION("two pages, needs to pad") {
    testPrint(
      mary_had_a_little_lamb + "\nBaa, baa, black sheep!\nHave you any wool?\n",
      2
    );
  }
}

void testPrint(
  std::string const& input,
  size_t expectedPageCount
) {
  std::istringstream inputs(input);
  std::ostringstream output;

  if (input.empty())
    inputs.setstate(std::ios_base::badbit);

  auto pageCount = stiX::print(
    filename,
    inputs,
    output,
    totalPageLength
  );

  REQUIRE(pageCount == expectedPageCount);

  auto pagedOutput = split(output.str());
  REQUIRE(pagedOutput.size() == expectedPageCount * totalPageLength);

  verifyHeader(expectedPageCount, pagedOutput);
  verifyPage(input, pagedOutput);
  verifyFooter(expectedPageCount, pagedOutput);
}

void verifyHeader(
  size_t expectedPageCount,
  std::vector<std::string> const& lines
) {
  for (size_t p = 0; p != expectedPageCount; ++p) {
    auto pageStart = totalPageLength * p;
    REQUIRE(lines[pageStart].empty());
    REQUIRE(lines[pageStart+1].empty());
    REQUIRE(lines[pageStart+2] == filename + " Page " + std::to_string(p+1));
    REQUIRE(lines[pageStart+3].empty());
    REQUIRE(lines[pageStart+4].empty());
  }
}

void verifyPage(
  std::string const& input,
  std::vector<std::string> const& lines
) {
  std::ostringstream allPages;

  size_t l = 0;
  while (l != lines.size()) {
    l += 5;
    for (auto i = 0; i != pageLength; ++i, ++l)
      allPages << lines[l] << '\n';
    l += 2;
  }

  auto all = allPages.str();
  REQUIRE(all.find(input) == 0);
}

void verifyFooter(
  size_t expectedPageCount,
  std::vector<std::string> const& lines
) {
  for (size_t p = 0; p != expectedPageCount; ++p) {
    auto pageEnd = totalPageLength * (p + 1);
    REQUIRE(lines[pageEnd - 2].empty());
    REQUIRE(lines[pageEnd - 1].empty());
  }
}
