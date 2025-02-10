#include "expression_engine.hpp"

#include <map>
#include <ranges>
#include <algorithm>
#include "../mp/support.hpp"
#include "../mp/predefined.hpp"
#include "../../../lib/chars.hpp"

namespace {
  using namespace std::string_literals;

  int multiply_fn(int lhs, int rhs) { return lhs * rhs; }
  int divide_fn(int lhs, int rhs) { return lhs / rhs; }
  int modulus_fn(int lhs, int rhs) { return lhs % rhs; }
  int add_fn(int lhs, int rhs) { return lhs + rhs; }
  int subtract_fn(int lhs, int rhs) { return lhs - rhs; }

  auto constexpr multiply = "*"s;
  auto constexpr divide = "/"s;
  auto constexpr modulus = "%"s;
  auto constexpr add = "+"s;
  auto constexpr subtract = "-"s;

  auto const operator_precedence = std::vector<std::vector<std::string_view>>{
    {multiply, divide,  modulus},
    {add,      subtract}
  };

  auto const fn = std::map<std::string, std::function<int(int, int)>>{
    {multiply, multiply_fn},
    {divide,   divide_fn},
    {modulus,  modulus_fn},
    {add,      add_fn},
    {subtract, subtract_fn}
  };

  std::vector<std::string>::iterator find_last_left_bracket(std::vector<std::string> &expression) {
    auto left_bracket_from_end = std::ranges::find(
      expression | std::views::reverse,
      pre::LeftParen);

    if (left_bracket_from_end == expression.rend())
      return expression.end();

    auto left_bracket = expression.begin() +
                        std::distance(left_bracket_from_end + 1, expression.rend());
    return left_bracket;
  }

  void evaluate_brackets(std::vector<std::string> &expression) {
    for (
      auto left_bracket = find_last_left_bracket(expression);
      left_bracket != expression.end();
      left_bracket = find_last_left_bracket(expression)) {
      auto right_bracket = std::find(left_bracket, expression.end(), pre::RightParen);

      auto subexpression = std::vector<std::string>(left_bracket + 1, right_bracket);
      auto [result, ok] = stiX::evaluate(subexpression);

      if (!ok)
        return;

      *left_bracket = std::to_string(result);
      expression.erase(left_bracket + 1, right_bracket + 1);
    }
  }

  void evaluate_ops(std::vector<std::string> &expression) {
    for (auto const &ops: operator_precedence) {
      for (auto op = std::ranges::find_first_of(expression, ops);
           op != expression.end();
           op = std::ranges::find_first_of(expression, ops)) {
        auto lhs = std::prev(op);
        auto rhs = std::next(op);

        auto [lhs_val, lhs_ok] = mp::to_int(*std::prev(op), 0);
        auto [rhs_val, rhs_ok] = mp::to_int(*std::next(op), 0);
        if (!lhs_ok || !rhs_ok)
          break;

        auto result = std::to_string(
          fn.at(*op)(lhs_val, rhs_val)
        );

        *lhs = result;
        expression.erase(op, std::next(rhs));
      }
    }
  }

  void strip_whitespace(std::vector<std::string> &expression) {
    auto e = std::remove_if(
      expression.begin(),
      expression.end(),
      [](std::string const& s) { return stiX::iswhitespace(s); }
    );
    expression.erase(e, expression.end());
  }
} // namespace

std::pair<int, bool> stiX::evaluate(std::vector<std::string> expression) {
  strip_whitespace(expression);

  evaluate_brackets(expression);
  evaluate_ops(expression);

  return (expression.size() == 1)
         ? mp::to_int(expression.front(), 0)
         : std::make_pair(-1, false);
}
