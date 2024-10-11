#include "macro.hpp"
#include "tokenizer.hpp"
#include "../../lib/chars.hpp"
#include <stdexcept>
#include <sstream>
#include <format>
#include <map>
#include <queue>

using namespace std::string_view_literals;

namespace {
  class push_back_buffer {
  public:
    push_back_buffer() { }

    bool token_available() const {
      return !buf_.empty();
    }

    std::string const& peek_token() const {
      return buf_.front();
    }

    std::string pop_token() {
      auto tok = buf_.front();
      buf_.pop();
      return tok;
    }

    void push_tokens(std::string const& str) {
      auto rstream = std::istringstream { str };
      auto rtok = stiX::tokenizer(rstream);

      for (auto const& r : rtok)
        buf_.push(r);
    }

  private:
    std::queue<std::string> buf_;
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

  class macro_processor {
  public:
    explicit macro_processor(std::istream& in);

    void process_to(std::ostream& out);

  private:
    bool token_available() const;
    std::string const& peek_token();
    std::string next_token();
    void expect_next(std::string_view expected);
    void skip_whitespace();

    void install_definition();
    std::pair<std::string, std::string> get_definition();

    bool is_macro(std::string const& tok) const;
    std::string const& macro_definition(std::string const& tok);
    void apply_macro(std::string const& tok);

    token_stream stream_;
    push_back_buffer buffer_;

    std::map<std::string, std::string> definitions_;
  };

  auto constexpr Define = "define"sv;
  auto constexpr LeftParen = "("sv;
  auto constexpr RightParen = ")"sv;
  auto constexpr EndOfInput = "<EOF>"sv;

macro_processor::macro_processor(std::istream& in) :
  stream_(in) {
}

void macro_processor::process_to(std::ostream& out) {
  while(token_available()) {
    auto token = next_token();

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

  throw std::runtime_error("Unexpected end of input");
} // next_token

std::string macro_processor::next_token() {
  if (buffer_.token_available())
    return buffer_.pop_token();

  if (stream_.token_available())
    return stream_.pop_token();

  throw std::runtime_error("Unexpected end of input");
} // next_token

void macro_processor::expect_next(std::string_view expected) {
  auto const next = token_available() ? next_token() : EndOfInput;
  if (expected != next)
    throw std::runtime_error(std::format("Expected {}", expected));
} // expect_next

bool iswhitespace(std::string const& token) {
  return token.size() == 1 && stiX::iswhitespace(token[0]);
}

void macro_processor::skip_whitespace() {
  while (token_available() && iswhitespace(peek_token()))
    next_token();
} // skip_whitespace

void macro_processor::install_definition() {
  auto [ def, replacement ] = get_definition();
  definitions_[def] = replacement;
}

std::pair<std::string, std::string> macro_processor::get_definition() {
  expect_next(LeftParen);
  auto def = next_token();
  if (!stiX::isalnum(def))
    throw std::runtime_error(std::format("{} is not alphanumeric", def));
  expect_next(",");
  skip_whitespace();
  auto replacement = std::string { };
  auto parens = 0;
  while (parens >= 0 && token_available()) {
    auto tok = next_token();

    parens -= (tok == RightParen);
    parens += (tok == LeftParen);

    replacement += tok;
  }
  if (!token_available())
    throw std::runtime_error("Expected )");
  replacement.pop_back();

  return { def, replacement };
}

bool macro_processor::is_macro(std::string const& tok) const {
  return definitions_.contains(tok);
}

std::string const& macro_processor::macro_definition(std::string const& tok) {
  return definitions_[tok];
}

void macro_processor::apply_macro(std::string const& tok) {
  buffer_.push_tokens(macro_definition(tok));
}

} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { in };
  macro.process_to(out);
}