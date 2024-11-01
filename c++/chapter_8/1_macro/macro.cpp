#include "macro.hpp"
#include "tokenizer.hpp"
#include "token_buffer.hpp"
#include "token_source.hpp"
#include "../../lib/chars.hpp"
#include <stdexcept>
#include <format>
#include <map>
#include <vector>
#include <numeric>

using namespace std::string_view_literals;
using namespace std::string_literals;

namespace {
  using stiX::token_seq;
  using stiX::token_buffer;
  using stiX::token_source;

  class macro_processor {
  public:
    void process_to(
      std::istream& in,
      std::ostream& out
    );

  private:
    void install_definition(token_source& source);
    std::pair<std::string, token_seq> get_definition(token_source& source);
    std::string get_definition_name(token_source& source);
    token_seq get_definition_replacement(token_source& source);

    void len_macro(token_source& source);

    bool is_macro(std::string const& tok) const;
    token_seq const& macro_definition(std::string const& tok);
    void apply_macro(std::string const& tok, token_source& source);
    std::vector<token_seq> gather_arguments(token_source& source);

    std::map<std::string, token_seq> definitions_;
  };

  auto constexpr Define = "define"sv;
  auto constexpr Len = "len"sv;
  auto constexpr LeftParen = "("sv;
  auto constexpr Comma = ","sv;
  auto constexpr RightParen = ")"sv;
  auto constexpr Dollar = "$"sv;

  bool is_whitespace(std::string const& token) {
    return token.size() == 1 && stiX::iswhitespace(token[0]);
  }

  void skip_whitespace(auto& tokens) {
    while (is_whitespace(tokens.peek_token()))
      tokens.pop_token();
  }

  token_seq parenthesised_sequence(auto& tokens) {
    if (tokens.peek_token() != LeftParen)
      return { };

    auto inner = token_seq { };
    auto parens = 0;
    do {
      auto tok = tokens.pop_token();

      parens -= (tok == RightParen);
      parens += (tok == LeftParen);

      inner += tok;
    } while (parens != 0 && tokens.token_available());

    if (parens != 0)
      throw std::runtime_error("Expected )");

    return inner;
  }

  bool is_next(auto& tokens, std::string_view expected) {
    return tokens.peek_token() == expected;
  }

  void expect_next(auto& tokens, std::string_view expected) {
    auto const& next = tokens.peek_token();
    if (expected != next)
      throw std::runtime_error(std::format("Expected {}", expected));
    tokens.pop_token();
  } // expect_next

  bool not_reached(auto& tokens, std::string_view end_marker) {
    return tokens.token_available() && tokens.peek_token() != end_marker;
  }

  token_seq gather_until(auto& tokens, std::string_view end_token) {
    auto arg = token_seq { };

    while (not_reached(tokens, end_token))
      if (tokens.peek_token() == LeftParen)
        arg += parenthesised_sequence(tokens);
      else
        arg += tokens.pop_token();

    return arg;
  }

  void drop_brackets(token_seq& tokens) {
    if (tokens.empty())
      return;

    tokens.pop_front();
    tokens.pop_back();
  }

  /////////////////////
  void macro_processor::process_to(
    std::istream& in,
    std::ostream& out
  ) {
    auto source = token_source { in };

    while(source.token_available()) {
      auto token = source.pop_token();

      if (token == Define)
        install_definition(source);
      else if (token == Len)
        len_macro(source);
      else if (is_macro(token))
        apply_macro(token, source);
      else
        out << token;
    }
  } // process_to

  void macro_processor::install_definition(token_source& source) {
    auto [ def, replacement ] = get_definition(source);
    definitions_[def] = replacement;
  }

  std::pair<std::string, token_seq> macro_processor::get_definition(
    token_source& source
  ) {
    expect_next(source, LeftParen);

    auto def = get_definition_name(source);

    expect_next(source, Comma);
    skip_whitespace(source);

    auto replacement = get_definition_replacement(source);

    return { def, replacement };
  }

  token_seq macro_processor::get_definition_replacement(token_source& source) {
    auto replacement = gather_until(source, RightParen);

    expect_next(source, RightParen);

    return replacement;
  }

  std::string macro_processor::get_definition_name(token_source& source) {
    auto def = source.pop_token();

    if (!stiX::isalnum(def))
      throw std::runtime_error(std::format("{} is not alphanumeric", def));

    return def;
  }

  void macro_processor::len_macro(token_source& source) {
    auto argument_tokens = parenthesised_sequence(source);

    drop_brackets(argument_tokens);

    auto length = std::accumulate(
      argument_tokens.begin(),
      argument_tokens.end(),
      size_t(),
      [](int total, std::string const& tok) {
        return total + tok.length();
      });

    source.push_token(std::to_string(length));
  }

  bool macro_processor::is_macro(std::string const& tok) const {
    return definitions_.contains(tok);
  }

  token_seq const& macro_processor::macro_definition(std::string const& tok) {
    return definitions_[tok];
  }

  int argument_index(std::string const& index_tok);
  token_seq argument_substitution(
    token_buffer& definition,
    std::vector<token_seq> const& arguments
  );
  token_buffer all_arguments(token_source& tokens);
  token_seq next_argument(token_buffer& tokens);

  void macro_processor::apply_macro(std::string const& tok, token_source& source) {
    auto const arguments = gather_arguments(source);
    auto definition = token_buffer { macro_definition(tok) };

    auto with_arg_substitution = token_seq { };
    while (definition.token_available()) {
      while (not_reached(definition, Dollar))
        with_arg_substitution += definition.pop_token();

      if (is_next(definition, Dollar))
        with_arg_substitution += argument_substitution(definition, arguments);
    }

    source.push_tokens(with_arg_substitution);
  }

  std::vector<token_seq> macro_processor::gather_arguments(token_source& source) {
    auto argument_tokens = all_arguments(source);
    if (!argument_tokens.token_available())
      return { };

    auto arguments = std::vector<token_seq> { };
    while(argument_tokens.token_available()) {
      skip_whitespace(argument_tokens);

      arguments.push_back(next_argument(argument_tokens));

      if (argument_tokens.token_available())
        expect_next(argument_tokens, Comma);
    }

    return arguments;
  }

  token_seq argument_substitution(
    token_buffer& definition,
    std::vector<token_seq> const& arguments
  ) {
    auto const dollar = definition.pop_token();
    auto const& index_tok = definition.peek_token();
    auto const index = argument_index(index_tok);

    if (index == -1)
      return { dollar };

    definition.pop_token();
    return (index < arguments.size())
           ? arguments[index]
           : token_seq { };
  }

  int argument_index(std::string const& index_tok) {
    auto index = 0;
    std::from_chars(index_tok.data(), index_tok.data() + index_tok.length(), index);
    return index - 1;
  }

  token_buffer all_arguments(token_source& tokens) {
    auto argument_tokens = parenthesised_sequence(tokens);

    drop_brackets(argument_tokens);

    return token_buffer { argument_tokens };
  }

  token_seq next_argument(token_buffer& tokens) {
    return gather_until(tokens, Comma);
  }

} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { };
  macro.process_to(in, out);
}