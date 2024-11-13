#ifndef STICPP_PREDEFINED_HPP
#define STICPP_PREDEFINED_HPP

#include <string_view>

namespace pre {
  using namespace std::string_view_literals;

  auto constexpr Define = "define"sv;
  auto constexpr Len = "len"sv;
  auto constexpr IfElse = "ifelse"sv;
  auto constexpr Expr = "expr"sv;
  auto constexpr Substr = "substr"sv;
  auto constexpr LeftParen = "("sv;
  auto constexpr Comma = ","sv;
  auto constexpr RightParen = ")"sv;
  auto constexpr Dollar = "$"sv;
  auto constexpr Grave = "`"sv;
  auto constexpr Apostrophe = "'"sv;
}
#endif //STICPP_PREDEFINED_HPP
