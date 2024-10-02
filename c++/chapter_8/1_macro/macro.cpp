#include "macro.hpp"
#include "tokenizer.hpp"

namespace {
  class macro_processor {
  public:
    macro_processor(std::istream& in);

    void process_to(std::ostream& out);

  private:
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
    auto token = *tok_;
    if (token == "define")
      definition();
    else
      out << *tok_;

    ++tok_;
  }
} // process_to

void macro_processor::definition() {
  while(*tok_ != ")")
    ++tok_;
}

} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { in };
  macro.process_to(out);
}