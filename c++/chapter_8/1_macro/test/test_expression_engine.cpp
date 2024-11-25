#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include <vector>
#include <map>
#include "../mp/support.hpp"
using namespace std::string_literals;

int multiply_fn(int lhs, int rhs) { return lhs * rhs; }
int divide_fn(int lhs, int rhs) { return lhs / rhs; }
int add_fn(int lhs, int rhs) { return lhs + rhs; }
int subtract_fn(int lhs, int rhs) { return lhs - rhs; }

auto constexpr multiply = "*"s;
auto constexpr divide = "/"s;
auto constexpr add = "+"s;
auto constexpr subtract = "-"s;

auto const operator_precedence = std::vector<std::vector<std::string_view>> {
  { multiply, divide },
  { add, subtract }
};

auto const fn = std::map<std::string, std::function<int(int,int)>> {
  { multiply, multiply_fn },
  { divide, divide_fn },
  { add, add_fn },
  { subtract, subtract_fn }
};

std::pair<int, bool> evaluate(std::vector<std::string> expression) {
  for(auto const& ops : operator_precedence) {
    for (auto mult = std::ranges::find_first_of(expression, ops);
         mult != expression.end();
         mult = std::ranges::find_first_of(expression, ops)) {
      auto lhs = std::prev(mult);
      auto rhs = std::next(mult);

      auto [lhs_val, lhs_ok] = mp::to_int(*std::prev(mult), 0);
      auto [rhs_val, rhs_ok] = mp::to_int(*std::next(mult), 0);
      if (!lhs_ok || !rhs_ok)
        break;

      auto result = std::to_string(
        fn.at(*mult)(lhs_val, rhs_val)
      );

      *lhs = result;
      expression.erase(mult, std::next(rhs));
    }
  }

  return (expression.size() == 1)
    ? mp::to_int(expression.front(), 0)
    : std::make_pair(-1, false);
}

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
    { { "5", "*", "3", "+", "9", "/", "3", "-", "8"}, 10 }
  };

  for (auto g: good) {
    auto name = std::ranges::fold_left(
      g.expression,
      std::string(),
      [](std::string const& l, std::string const& r) { return l+" "+r; });
    DYNAMIC_SECTION(name + " = " + std::to_string(g.expected)) {
      auto [ result, ok ] = evaluate(g.expression);

      REQUIRE(true == ok);
      REQUIRE(g.expected == result);
    }
  }
}