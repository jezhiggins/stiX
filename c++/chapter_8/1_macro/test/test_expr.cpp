#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "test.hpp"

TEST_CASE("expr()") {
  auto good = std::vector<good_case>{
    { "expr is just expr",
      "expr",
      "expr" },
    { "expr(constant)",
      "expr(6) is 6",
      "6 is 6" },
    { "expr(defined)",
      "define(number, 99)expr(number) is 99",
      "99 is 99" }
  };
  auto warning = std::vector<warning_case> {
    { "too few args to expr", "expr()", "0", "Warning: too few arguments to `expr'\n"},
    { "too many args to expr", "expr(2,99)", "2", "Warning: excess arguments to `expr' ignored\n" },
    { "bad argument", "expr(fruit)", "", "Warning: bad expression in `expr': fruit\n" }
  };
  auto bad = std::vector<bad_case> {
    { "expr(99", "Expected )" }
  };

  build_good_tests(good);
  build_warning_tests(warning);
  build_bad_tests(bad);
}