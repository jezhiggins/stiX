#include "macro.hpp"
#include "tokenizer.hpp"
#include "../../lib/chars.hpp"
#include <stdexcept>
#include <sstream>
#include <format>
#include <map>
#include <queue>
#include <algorithm>

using namespace std::string_view_literals;
using namespace std::string_literals;

namespace {
  using token_seq = std::deque<std::string>;

  class token_buffer {
  public:
    token_buffer() { }
    token_buffer(token_seq const& tokens): buf_(tokens) { }

    bool token_available() const {
      return !buf_.empty();
    }

    std::string const& peek_token() const {
      return buf_.front();
    }

    std::string pop_token() {
      auto tok = buf_.front();
      buf_.pop_front();
      return tok;
    }

    void push_tokens(token_seq const& tokens) {
      std::ranges::copy(tokens, std::back_inserter(buf_));
    }

  private:
    token_seq buf_;
  };

  class token_stream {
  public:
    explicit token_stream(std::istream& in):
      tokenizer_(in),
      tok_(tokenizer_.begin()) {
    }

    bool token_available() const {
      return tok_ != tokenizer_.end();
    }

    std::string const& peek_token() const {
      return *tok_;
    }

    std::string pop_token() {
      auto tok = *tok_;
      ++tok_;
      return tok;
    }

  private:
    stiX::tokenizer tokenizer_;
    stiX::stream_token_iterator tok_;
  };

  int argument_index(std::string const& index_tok);

  class macro_processor {
  public:
    explicit macro_processor(std::istream& in);

    void process_to(std::ostream& out);

  private:
    bool token_available() const;
    std::string const& peek_token();
    std::string pop_token();
    void expect_next(std::string_view expected);
    token_seq next_parens_sequence(bool half_open);

    void install_definition();
    std::pair<std::string, token_seq> get_definition();
    std::string get_definition_name();
    token_seq get_definition_replacement();

    bool is_macro(std::string const& tok) const;
    token_seq const& macro_definition(std::string const& tok);
    void apply_macro(std::string const& tok);
    std::vector<token_seq> gather_arguments();

    token_stream stream_;
    token_buffer buffer_;

    std::map<std::string, token_seq> definitions_;

    friend void skip_whitespace(auto&);
  };

  auto constexpr Define = "define"sv;
  auto constexpr LeftParen = "("sv;
  auto constexpr Comma = ","sv;
  auto constexpr RightParen = ")"sv;
  auto const EndOfInput = "<EOF>"s;

  bool iswhitespace(std::string const& token) {
    return token.size() == 1 && stiX::iswhitespace(token[0]);
  }

  void skip_whitespace(auto& tokens) {
    while (tokens.token_available() && iswhitespace(tokens.peek_token()))
      tokens.pop_token();
  }
  void skip_whitespace(macro_processor* mp) {
    skip_whitespace(*mp);
  }


  macro_processor::macro_processor(std::istream& in) :
  stream_(in) {
}

void macro_processor::process_to(std::ostream& out) {
  while(token_available()) {
    auto token = pop_token();

    if (token == Define)
      install_definition();
    else if (is_macro(token))
      apply_macro(token);
    else
      out << token;
  }
} // process_to

bool macro_processor::token_available() const {
  return buffer_.token_available() || stream_.token_available();
}

std::string const& macro_processor::peek_token() {
  if (buffer_.token_available())
    return buffer_.peek_token();

  if (stream_.token_available())
    return stream_.peek_token();

  return EndOfInput;
} // pop_token

std::string macro_processor::pop_token() {
  if (buffer_.token_available())
    return buffer_.pop_token();

  if (stream_.token_available())
    return stream_.pop_token();

  throw std::runtime_error("Unexpected end of input");
} // pop_token

void macro_processor::expect_next(std::string_view expected) {
  auto const next = token_available() ? pop_token() : EndOfInput;
  if (expected != next)
    throw std::runtime_error(std::format("Expected {}", expected));
} // expect_next

token_seq macro_processor::next_parens_sequence(bool half_open) {
  auto replacement = token_seq { };
  if (!half_open) {
    if (peek_token() != LeftParen)
      return replacement;
    pop_token();
  }

  auto parens = 0;
  while (parens >= 0 && token_available()) {
    auto tok = pop_token();

    parens -= (tok == RightParen);
    parens += (tok == LeftParen);

    replacement.push_back(tok);
  }
  if (parens >= 0)
    throw std::runtime_error("Expected )");

  replacement.pop_back();

  return replacement;
}

void macro_processor::install_definition() {
  auto [ def, replacement ] = get_definition();
  definitions_[def] = replacement;
}

std::pair<std::string, token_seq> macro_processor::get_definition() {
  expect_next(LeftParen);

  auto def = get_definition_name();

  expect_next(Comma);
  skip_whitespace(this);

  auto replacement = get_definition_replacement();

  return { def, replacement };
}

token_seq macro_processor::get_definition_replacement() {
  return next_parens_sequence(true);
}

std::string macro_processor::get_definition_name() {
  auto def = pop_token();

  if (!stiX::isalnum(def))
    throw std::runtime_error(std::format("{} is not alphanumeric", def));

  return def;
}

bool macro_processor::is_macro(std::string const& tok) const {
  return definitions_.contains(tok);
}

token_seq const& macro_processor::macro_definition(std::string const& tok) {
  return definitions_[tok];
}

void macro_processor::apply_macro(std::string const& tok) {
  auto arguments = gather_arguments();
  auto definition = token_buffer {macro_definition(tok) };

  auto definition_with_arg_substitution = token_seq { };
  auto defs = std::back_inserter(definition_with_arg_substitution);
  while (definition.token_available()) {
    auto const tok = definition.pop_token();

    if (tok != "$" || !definition.token_available())
      defs = tok;
    else {
      auto const index_tok = definition.pop_token();
      auto const index = argument_index(index_tok);

      if (index != -1) {
        if (index < arguments.size())
          std::ranges::copy(arguments[index], defs);
      }
      else { // bad index, so just pop it in there
        defs = tok;
        defs = index_tok;
      }
    }
  }

  buffer_.push_tokens(definition_with_arg_substitution);
}

int argument_index(std::string const& index_tok) {
  if (index_tok.length() != 1)
    return -1;

  auto index = 0;
  std::from_chars(index_tok.data(), index_tok.data() + index_tok.length(), index);

  return index-1;
}

std::vector<token_seq> macro_processor::gather_arguments() {
  auto arguments = std::vector<token_seq> { };
  auto in_brackets = token_buffer { next_parens_sequence(false) };

  while(in_brackets.token_available()) {
    skip_whitespace(in_brackets);

    arguments.push_back({ });
    while(in_brackets.token_available() && in_brackets.peek_token() != Comma)
      arguments.back().push_back(in_brackets.pop_token());

    if (in_brackets.token_available())
      in_brackets.pop_token(); // must be a comma
  }

  return arguments;
}

} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { in };
  macro.process_to(out);
}