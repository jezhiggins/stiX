#include "../../testlib/testlib.h"

#include "echo.h"

char const* argv[] = {
    "fruit",
    "apple",
    "banana",
    "plum"
};

void testEcho(int n);

TEST_CASE("Chapter 2 - echo") {
    testEcho(1);
    testEcho(2);
    testEcho(3);
    testEcho(4);
}

std::string testLabel(int n);
std::string expected(int n);

void testEcho(int n) {
  DYNAMIC_SECTION("echo(" << testLabel(n) << ")") {
    std::ostringstream os;

    stiX::echo(n, argv, os);

    REQUIRE(os.str() == expected(n));
  }
}

std::string testLabel(int n) {
  std::ostringstream l;
  l << '[';
  for (int i = 1; i != n; ++i) {
      if (i > 1)
        l << ',';
      l << argv[i];
  }
  l << ']';
  return l.str();
}

std::string expected(int n) {
  std::ostringstream e;
  for (int i = 1; i != n; ++i) {
      if (i > 1)
        e << ' ';
      e << argv[i];
  }
  return e.str();
}