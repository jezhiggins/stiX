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
      "99 is 99" },
    { "simple multiplication",
      "expr(3 * 3) is 9",
      "9 is 9" },
    { "simple division",
      "expr(3 / 3) is 1",
      "1 is 1" },
    { "simple modulus",
      "expr(10 % 3) is 1",
      "1 is 1" },
    { "simple multiplication, with second term an eval",
      "expr(3 * expr(2 * 3)) is 18",
      "18 is 18" },
    { "two multiplications",
      "expr(3 * 3 * 2) is 18",
      "18 is 18" },
    { "lots of parentheses",
      "expr(3 * ((1+2) * (1+1+(1-1)))) is 18",
      "18 is 18" },
    { "expression with precedence",
      "expr(5*2+2) = 12",
      "12 = 12" }
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