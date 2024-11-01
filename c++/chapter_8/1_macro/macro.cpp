#include "macro.hpp"
#include "tokenizer.hpp"
#include "token_buffer.hpp"
#include "token_source.hpp"
#include "../../lib/chars.hpp"
#include <stdexcept>
#include <format>
#include <functional>
#include <map>
#include <vector>
#include <sstream>

using namespace std::string_view_literals;
using namespace std::string_literals;

namespace {
  using stiX::token_seq;
  using stiX::token_buffer;
  using stiX::token_source;

  class macro_processor {
  public:
    void process(
      std::istream& in,
      std::ostream& out
    );

  private:
    using token_sink = std::function<void(std::string const&)>;
    using macro_fn = void (macro_processor::*)(
      std::string const&,
      token_source&,
      token_sink
    );

    void frame(token_source&& source, std::ostream& out);
    void frame(token_source&& source, token_seq& result);
    void frame(token_source&& source, token_sink sink);
    std::string sub_frame_to_string(token_source&& in);
    token_seq sub_frame_to_seq(token_source&& in);

    void define_replacement(std::string const&, token_source&, token_sink);
    std::pair<std::string, token_seq> name_and_replacement(token_source& source);

    void len_macro(std::string const&, token_source&, token_sink);
    void quoted_sequence(std::string const&, token_source&, token_sink);
    void apply_replacement(std::string const&,token_source&, token_sink);

    void install_macro(std::string_view name, macro_fn fn) {
      macros_[std::string(name)] = fn;
    }
    bool is_macro(std::string const& tok) const {
      return macros_.contains(tok);
    }
    macro_fn macro_function(std::string const& tok) {
      return macros_[tok];
    }

    std::map<std::string, macro_fn> macros_;
    std::map<std::string, token_seq> replacements_;
  };

  auto constexpr Define = "define"sv;
  auto constexpr Len = "len"sv;
  auto constexpr LeftParen = "("sv;
  auto constexpr Comma = ","sv;
  auto constexpr RightParen = ")"sv;
  auto constexpr Dollar = "$"sv;
  auto constexpr Grave = "`"sv;
  auto constexpr Apostrophe = "'"sv;

  bool is_whitespace(std::string const& token) {
    return token.size() == 1 && stiX::iswhitespace(token[0]);
  }

  void skip_whitespace(token_source& tokens) {
    while (is_whitespace(tokens.peek_token()))
      tokens.pop_token();
  }

  token_seq parenthesised_sequence(token_source& tokens) {
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

  void drop_brackets(token_seq& tokens) {
    if (tokens.empty())
      return;

    tokens.pop_front();
    tokens.pop_back();
  }

  token_source all_arguments(token_source& tokens) {
    auto argument_tokens = parenthesised_sequence(tokens);
    drop_brackets(argument_tokens);
    return token_source { argument_tokens };
  }

  bool is_next(token_source& tokens, std::string_view expected) {
    return tokens.peek_token() == expected;
  }

  void check_next(token_source& tokens, std::string_view expected) {
    if (!is_next(tokens, expected))
      throw std::runtime_error(std::format("Expected {}", expected));
  } // check_next

  void expect_next(token_source& tokens, std::string_view expected) {
    check_next(tokens, expected);
    tokens.pop_token();
  } // expect_next

  bool not_reached(token_source& tokens, std::string_view end_marker) {
    return tokens.token_available() && tokens.peek_token() != end_marker;
  }

  /////////////////////
  void macro_processor::process(
    std::istream& in,
    std::ostream& out
  ) {
    install_macro(Define, &macro_processor::define_replacement);
    install_macro(Len, &macro_processor::len_macro);
    install_macro(Grave, &macro_processor::quoted_sequence);

    frame(token_source { in }, out);
  }

  void macro_processor::frame(
    token_source&& source,
    std::ostream& out
  ) {
    frame(
      std::move(source),
      [&out](std::string const& token) { out << token; }
    );
  }

  void macro_processor::frame(
    token_source&& source,
    token_seq& result
  ) {
    frame(
      std::move(source),
      [&result](std::string const& token) { result.push_back(token); }
    );
  }

  void macro_processor::frame(
    token_source&& source,
    token_sink sink)
  {
    while(source.token_available()) {
      auto token = source.pop_token();

      if (is_macro(token)) {
        auto fn = macro_function(token);
        std::invoke(fn, this, token, source,sink);
      }
      else
        sink(token);
    }
  } // process

  std::string macro_processor::sub_frame_to_string(token_source&& in) {
    auto sink = std::ostringstream { };
    frame(std::move(in), sink);
    return sink.str();
  }

  token_seq macro_processor::sub_frame_to_seq(token_source&& in) {
    auto sink = token_seq { };
    frame(std::move(in), sink);
    return sink;
  }

  void macro_processor::define_replacement(
      std::string const&,
      token_source& source,
      token_sink
  ) {
    auto [ def, replacement ] = name_and_replacement(source);
    replacements_[def] = replacement;
    macros_[def] = &macro_processor::apply_replacement;
  }

  std::string definition_name(token_source& source);

  std::pair<std::string, token_seq> macro_processor::name_and_replacement(
    token_source& source
  ) {
    check_next(source, LeftParen);

    auto define_source = all_arguments(source);

    auto def = definition_name(define_source);
    auto replacement = sub_frame_to_seq(std::move(define_source));
    return { def, replacement };
  }

  std::string definition_name(token_source& source) {
    auto def = source.pop_token();
    if (!stiX::isalnum(def))
      throw std::runtime_error(std::format("{} is not alphanumeric", def));
    skip_whitespace(source);
    expect_next(source, Comma);
    skip_whitespace(source);
    return def;
  }

  void macro_processor::len_macro(
    std::string const&,
    token_source& source,
    token_sink
  ) {
    auto expansion = sub_frame_to_string(
      all_arguments(source)
    );

    source.push_token(std::to_string(expansion.size()));
  }

  void macro_processor::quoted_sequence(
    std::string const&,
    token_source& source,
    token_sink sink
  ) {
    while(not_reached(source, Apostrophe))
      sink(source.pop_token());
    source.pop_token();
  }

  int argument_index(std::string const& index_tok);
  token_seq argument_substitution(
    token_source& definition,
    std::vector<token_seq> const& arguments
  );
  token_seq next_argument(token_source& tokens);
  std::vector<token_seq> gather_arguments(token_source& source);

  void macro_processor::apply_replacement(
    std::string const& token,
    token_source& source,
    token_sink
  ) {
    auto const arguments = gather_arguments(source);
    auto definition = token_source { replacements_[token] };

    auto with_arg_substitution = token_seq { };
    while (definition.token_available()) {
      while (not_reached(definition, Dollar))
        with_arg_substitution += definition.pop_token();

      if (is_next(definition, Dollar))
        with_arg_substitution += argument_substitution(definition, arguments);
    }

    source.push_tokens(with_arg_substitution);
  }

  std::vector<token_seq> gather_arguments(token_source& source) {
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
    token_source& definition,
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

  token_seq next_argument(token_source& tokens) {
    auto arg = token_seq { };

    while (not_reached(tokens, Comma))
      if (tokens.peek_token() == LeftParen)
        arg += parenthesised_sequence(tokens);
      else
        arg += tokens.pop_token();

    return arg;
  }

} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { };
  macro.process(in, out);
}