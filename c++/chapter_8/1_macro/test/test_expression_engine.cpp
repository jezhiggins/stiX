#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "../source/expression_engine.hpp"

struct good_expr {
  std::vector<std::string> expression;
  int expected;
};

TEST_CASE("expression engine") {
  auto good = std::vector<good_expr> {
    { { "2" }, 2 },
    { { "2", "*", "3" }, 6 },
    { { "2", "*", "3", "*", "4" }, 24 },
    { { "2", "*", "3", "*", "4", "*", "5" }, 120 },
    { { "10", "/", "2" }, 5 },
    { { "13", "/", "4" }, 3 },
    { { "13", "/", "3", "/", "2" }, 2 },
    { { "45", "/", "3", "*", "3" }, 45},
    { { "2", "+", "3" }, 5 },
    { { "2", "+", "3", "*", "5" }, 17 },
    { { "5", "-", "3" }, 2 },
    { { "5", "-", "3", "+", "2" }, 4 },
    { { "5", "*", "3", "+", "9", "/", "3", "-", "8"}, 10 },
    { { "2", "+", "3", "*", "5" }, 17 },
    { { "(", "2", "+", "3", ")", "*", "5" }, 25 },
    { { "2", "+", "(", "3", "*", "5", ")" }, 17 },
    { { "2", "+", "(", "3", "*", "1", "+", "4", ")" }, 9 },
    { { "2", "+", "(", "3", "*", "(", "1", "+", "4", ")", ")" }, 17 }
  };

  for (auto g: good) {
    auto name = std::ranges::fold_left(
      g.expression,
      std::string(),
      [](std::string const& l, std::string const& r) { return l+" "+r; });
    DYNAMIC_SECTION(name + " = " + std::to_string(g.expected)) {
      auto [ result, ok ] = stiX::evaluate(g.expression);

      REQUIRE(true == ok);
      REQUIRE(g.expected == result);
    }
  }
}