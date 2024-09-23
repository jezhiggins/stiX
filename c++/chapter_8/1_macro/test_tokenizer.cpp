#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"
using namespace std::string_literals;

namespace stiX {
  class stream_token_iterator {
  public:
    stream_token_iterator(std::istream& input) :
      input_(&input) {
      next_token();
    }
    stream_token_iterator() :
      input_() {
    }

    std::string operator*() { return token_; }

  private:
    void next_token() {
      token_ = "one";
    }

    std::istream* input_;
    std::string token_;
  };

  class tokenizer {
  public:
    tokenizer(std::istream& input) :
      input_(input) {
    }

    stream_token_iterator begin() const {
      return stream_token_iterator(input_);
    }
    stream_token_iterator end() const {
      return stream_token_iterator();
    }

  private:
    std::istream& input_;
  };
}

TEST_CASE("tokenizer") {
  auto input = std::istringstream("one");

  auto tok = stiX::tokenizer(input);
  auto toki = tok.begin();
  REQUIRE(*toki == "one"s);
}