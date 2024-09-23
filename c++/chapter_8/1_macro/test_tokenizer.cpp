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
    stream_token_iterator& operator++() {
      token();
      return *this;
    }

  private:
    void token() {
      token_.clear();
      while(input_available() && token_.empty())
        token_ = next_token();
    } // token

    std::string next_token() {
      std::string tok;

      while(input_available() && stiX::isalnum(input_->peek()))
        tok += static_cast<char>(input_->get());

      return tok;
    } // next_token

    bool input_available() {
      if (input_ && input_->good())
        return true;
      input_ = std::nullptr_t { };
      return false;
    }

    std::istream* input_;
    std::string token_;

    friend bool operator==(stream_token_iterator const&, stream_token_iterator const&);
  };

  bool operator==(stream_token_iterator const& lhs, stream_token_iterator const& rhs) {
    return !lhs.input_ && !rhs.input_;
  }

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
    ++toki;
    REQUIRE(toki == tok.end());
  }
}