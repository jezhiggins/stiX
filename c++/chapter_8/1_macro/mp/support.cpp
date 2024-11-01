#include <stdexcept>
#include <format>
#include "support.hpp"
#include "../token_seq.hpp"
#include "../token_source.hpp"
#include "predefined.hpp"
#include "../../../lib/chars.hpp"

namespace mp {
  bool is_whitespace(std::string const &token) {
    return token.size() == 1 && stiX::iswhitespace(token[0]);
  }

  void skip_whitespace(stiX::token_source &tokens) {
    while (is_whitespace(tokens.peek_token()))
      tokens.pop_token();
  }

  stiX::token_seq parenthesised_sequence(stiX::token_source &tokens) {
    if (tokens.peek_token() != pre::LeftParen)
      return {};

    auto inner = stiX::token_seq{};
    auto parens = 0;
    do {
      auto tok = tokens.pop_token();

      parens -= (tok == pre::RightParen);
      parens += (tok == pre::LeftParen);

      inner += tok;
    } while (parens != 0 && tokens.token_available());

    if (parens != 0)
      throw std::runtime_error("Expected )");

    return inner;
  }

  void drop_brackets(stiX::token_seq &tokens) {
    if (tokens.empty())
      return;

    tokens.pop_front();
    tokens.pop_back();
  }

  stiX::token_source all_arguments(stiX::token_source &tokens) {
    auto argument_tokens = parenthesised_sequence(tokens);
    drop_brackets(argument_tokens);
    return stiX::token_source{argument_tokens};
  }

  bool is_next(stiX::token_source &tokens, std::string_view expected) {
    return tokens.peek_token() == expected;
  }

  void check_next(stiX::token_source &tokens, std::string_view expected) {
    if (!is_next(tokens, expected))
      throw std::runtime_error(std::format("Expected {}", expected));
  } // check_next

  void expect_next(stiX::token_source &tokens, std::string_view expected) {
    check_next(tokens, expected);
    tokens.pop_token();
  } // expect_next

  bool not_reached(stiX::token_source &tokens, std::string_view end_marker) {
    return tokens.token_available() && tokens.peek_token() != end_marker;
  }

  std::string definition_name(stiX::token_source& source) {
    auto def = source.pop_token();
    if (!stiX::isalnum(def))
      throw std::runtime_error(std::format("{} is not alphanumeric", def));
    skip_whitespace(source);
    expect_next(source, pre::Comma);
    skip_whitespace(source);
    return def;
  }

  std::vector<stiX::token_seq> gather_arguments(
    stiX::token_source& source
  ) {
    auto argument_tokens = all_arguments(source);
    if (!argument_tokens.token_available())
      return { };

    auto arguments = std::vector<stiX::token_seq> { };
    while(argument_tokens.token_available()) {
      skip_whitespace(argument_tokens);

      arguments.push_back(next_argument(argument_tokens));

      if (argument_tokens.token_available())
        expect_next(argument_tokens, pre::Comma);
    }

    return arguments;
  }

  stiX::token_seq argument_substitution(
    stiX::token_source& definition,
    std::vector<stiX::token_seq> const& arguments
  ) {
    auto const dollar = definition.pop_token();
    auto const& index_tok = definition.peek_token();
    auto const index = argument_index(index_tok);

    if (index == -1)
      return { dollar };

    definition.pop_token();
    return (index < arguments.size())
           ? arguments[index]
           : stiX::token_seq { };
  }

  int argument_index(std::string const& index_tok) {
    auto index = 0;
    std::from_chars(index_tok.data(), index_tok.data() + index_tok.length(), index);
    return index - 1;
  }

  stiX::token_seq next_argument(stiX::token_source& tokens) {
    auto arg = stiX::token_seq { };

    while (not_reached(tokens, pre::Comma))
      if (tokens.peek_token() == pre::LeftParen)
        arg += parenthesised_sequence(tokens);
      else
        arg += tokens.pop_token();

    return arg;
  }
} // namespace mp