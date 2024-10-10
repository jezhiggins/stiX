#include "macro.hpp"
#include "tokenizer.hpp"
#include "../../lib/chars.hpp"
#include <stdexcept>
#include <format>
#include <map>

namespace {
  class macro_processor {
  public:
    macro_processor(std::istream& in);

    void process_to(std::ostream& out);

  private:
    bool token_available() const;
    std::string next_token();
    void expect_next(std::string const& expected);
    void skip_whitespace();

    void install_definition();
    std::pair<std::string, std::string> get_definition();

    bool is_macro(std::string const& tok) const;
    std::string const& macro_definition(std::string const& tok);

    stiX::tokenizer tokenizer_;
    stiX::stream_token_iterator tok_;

    std::map<std::string, std::string> definitions_;
  };

macro_processor::macro_processor(std::istream& in) :
  tokenizer_(in),
  tok_(tokenizer_.begin()) {
}

void macro_processor::process_to(std::ostream& out) {
  while(token_available()) {
    auto token = next_token();
    if (token == "define")
      install_definition();
    else if (is_macro(token))
      out << macro_definition(token);
    else
      out << token;
  }
} // process_to

bool macro_processor::token_available() const {
  return tok_ != tokenizer_.end();
}

std::string macro_processor::next_token() {
  if (!token_available())
    throw std::runtime_error("Unexpected end of input");

  auto token = *tok_;
  ++tok_;
  return token;
} // next_token

void macro_processor::expect_next(std::string const& expected) {
  auto next = token_available() ? next_token() : "<EOF>";
  if (expected != next)
    throw std::runtime_error(std::format("Expected {}", expected));
} // expect_next

bool iswhitespace(std::string const& token) {
  return token.size() == 1 && stiX::iswhitespace(token[0]);
}

void macro_processor::skip_whitespace() {
  while (token_available() && iswhitespace(*tok_))
    ++tok_;
} // skip_whitespace

void macro_processor::install_definition() {
  auto [ def, replacement ] = get_definition();
  definitions_[def] = replacement;
}

std::pair<std::string, std::string> macro_processor::get_definition() {
  expect_next("(");
  auto def = next_token();
  if (!stiX::isalnum(def))
    throw std::runtime_error(std::format("{} is not alphanumeric", def));
  expect_next(",");
  skip_whitespace();
  auto replacement = std::string { };
  auto parens = 0;
  while (parens >= 0 && token_available()) {
    auto tok = next_token();
    if (tok == ")")
      --parens;
    if (tok == "(")
      ++parens;
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

} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { in };
  macro.process_to(out);
}