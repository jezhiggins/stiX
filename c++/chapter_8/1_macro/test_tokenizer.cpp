#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"
#include "../../lib/chars.hpp"

using namespace std::string_literals;

namespace stiX {
  class stream_token_iterator {
  public:
    stream_token_iterator(std::istream& input) :
      input_(&input) {
      token();
    }
    stream_token_iterator() :
      input_() {
    }

    std::string operator*() { return token_; }

  private:
    void token() {
      token_.clear();
      if (!*input_) {
        input_ = std::nullptr_t {};
        return;
      }
      while(*input_ && token_.empty())
        token_ = next_token();
    } // token

    std::string next_token() {
      std::string tok;

      while(*input_ && stiX::isalnum(input_->peek()))
        tok += static_cast<char>(input_->get());

      return tok;
    } // next_token

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
  SECTION("one") {
    auto input = std::istringstream("one");

    auto tok = stiX::tokenizer(input);
    auto toki = tok.begin();
    REQUIRE(*toki == "one"s);
  }
}