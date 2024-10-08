#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"
#include "tokenizer.hpp"

using namespace std::string_literals;

void test_tokenizer(std::string in, std::vector<std::string> expected) {
  SECTION(in) {
    auto input = std::istringstream(in);

    auto tok = stiX::tokenizer(input);
    auto e = expected.begin();
    for (auto token: tok) {
      if (e == expected.end())
        FAIL("Too many tokens");
      REQUIRE(token == *e);
      ++e;
    }
    if (e != expected.end())
      FAIL("Not enough tokens");
  }
}

TEST_CASE("tokenizer") {
  test_tokenizer("one", { "one" });
  test_tokenizer("one two three", {
    "one", " ", "two", " ", "three"
  });
  test_tokenizer("define(x, 1)", {
    "define",
    "(",
    "x",
    ",",
    " ",
    "1",
    ")"
  });
}
