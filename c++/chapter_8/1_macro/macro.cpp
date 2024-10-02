#include "macro.hpp"
#include "tokenizer.hpp"
#include <stdexcept>

namespace {
  class macro_processor {
  public:
    macro_processor(std::istream& in);

    void process_to(std::ostream& out);

  private:
    std::string next_token();
    void definition();

    stiX::tokenizer tokenizer_;
    stiX::stream_token_iterator tok_;
  };

macro_processor::macro_processor(std::istream& in) :
  tokenizer_(in),
  tok_(tokenizer_.begin()) {
}

void macro_processor::process_to(std::ostream& out) {
  while(tok_ != tokenizer_.end()) {
    auto token = next_token();
    if (token == "define")
      definition();
    else
      out << token;
  }
} // process_to

std::string macro_processor::next_token() {
  if (tok_ == tokenizer_.end())
    throw std::runtime_error("Unexpected end of input");

  auto token = *tok_;
  ++tok_;
  return token;
} // next_token

void macro_processor::definition() {
  if (next_token() != "(")
    throw std::runtime_error("Expected (");

  while (next_token() != ")");
}

} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { in };
  macro.process_to(out);
}