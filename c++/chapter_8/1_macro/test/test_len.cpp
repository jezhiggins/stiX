#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "test.hpp"

TEST_CASE("len()") {
  auto good = std::vector<good_case>{
    { "len of a single token",
      "len(string) is 6",
      "6 is 6" },
    { "len of a multiple tokens",
      "len(hello Brian Kernighan and PJ Plauger) is 36",
      "36 is 36" },
    { "len of a len",
      "len(len(hello world)) == len(11)",
      "2 == 2" },
    { "len of a define, define is visible afterwards",
      "len(define(hello, world)) hello",
      "0 world" }
  };
  auto warning = std::vector<warning_case> {
    { "too many args to len", "len(x,y,z) x", "1 x", "Warning: excess arguments to `len' ignored\n" },
  };
  auto bad = std::vector<bad_case> {
    { "len(a string that just flops off the end", "Expected )" }
  };

  build_good_tests(good);
  build_warning_tests(warning);
  build_bad_tests(bad);
}