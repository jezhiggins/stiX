#include <stdexcept>
#include <format>
#include "support.hpp"
#include "../source/token_seq.hpp"
#include "../source/token_stream.hpp"
#include "predefined.hpp"
#include "../../../lib/chars.hpp"

namespace mp {
  bool do_not_evaluate(
      std::string const& macro,
      stiX::token_stream& source,
      token_sink& sink) {
    auto is_bad = !is_next(source, pre::LeftParen);
    if (is_bad)
      sink(macro);
    return is_bad;
  }

  bool is_whitespace(std::string const &token) {
    return token.size() == 1 && stiX::iswhitespace(token[0]);
  }

  void skip_whitespace(stiX::token_stream& tokens) {
    while (is_whitespace(tokens.peek_token()))
      tokens.pop_token();
  }


  stiX::token_seq bracketed_sequence(
      stiX::token_stream& tokens,
      std::string_view opening,
      std::string_view closing) {
    if (tokens.peek_token() != opening)
      return {};

    auto inner = stiX::token_seq{};
    auto parens = 0;
    do {
      auto tok = tokens.pop_token();

      parens -= (tok == closing);
      parens += (tok == opening);

      inner += tok;
    } while (parens != 0 && tokens.token_available());

    if (parens != 0)
      throw std::runtime_error(std::format("Expected {}", closing));

    return inner;
  }

  stiX::token_seq parenthesised_sequence(stiX::token_stream& tokens) {
    return bracketed_sequence(tokens, pre::LeftParen, pre::RightParen);
  }

  void drop_bracketing(stiX::token_seq &tokens) {
    if (tokens.empty())
      return;

    tokens.pop_front();
    tokens.pop_back();
  }

  stiX::token_stream all_arguments(stiX::token_stream& tokens) {
    auto argument_tokens = parenthesised_sequence(tokens);
    drop_bracketing(argument_tokens);
    return stiX::token_stream { argument_tokens };
  }

  bool is_next(stiX::token_stream& tokens, std::string_view expected) {
    return tokens.peek_token() == expected;
  }

  void check_next(stiX::token_stream& tokens, std::string_view expected) {
    if (!is_next(tokens, expected))
      throw std::runtime_error(std::format("Expected {}", expected));
  } // check_next

  void expect_next(stiX::token_stream& tokens, std::string_view expected) {
    check_next(tokens, expected);
    tokens.pop_token();
  } // expect_next

  bool not_reached(stiX::token_stream& tokens, std::string_view end_marker) {
    return tokens.token_available() && tokens.peek_token() != end_marker;
  }

  std::vector<stiX::token_seq> gather_arguments(
    stiX::token_stream& source
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
    stiX::token_stream& definition,
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

  stiX::token_seq next_argument(stiX::token_stream& tokens) {
    auto arg = stiX::token_seq { };

    while (not_reached(tokens, pre::Comma))
      if (tokens.peek_token() == pre::LeftParen)
        arg += parenthesised_sequence(tokens);
      else
        arg += tokens.pop_token();

    return arg;
  }

  std::pair<int, bool> int_arg(std::vector<std::string> const& args, size_t index, int def) {
    if (index >= args.size())
      return { def, true };

    auto value = def;
    auto const& arg = args[index];
    auto [_, ec] = std::from_chars(arg.data(), arg.data() + arg.length(), value);

    return { value, ec == std::errc() };
  }
} // namespace mp