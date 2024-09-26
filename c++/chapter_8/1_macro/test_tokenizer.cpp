#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"
#include "../../lib/chars.hpp"

using namespace std::string_literals;

namespace stiX {
  constexpr auto eof = std::char_traits<char>::eof();

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
      token_ = next_token();
    } // token

    std::string next_token() {
      if(input_available() && !stiX::isalnum(peek()))
        return std::string { get() };

      std::string tok;
      while(input_available() && stiX::isalnum(peek()))
        tok += get();
      return tok;
    } // next_token

    bool input_available() {
      if (!(input_ && input_->good() && peek() != eof))
        input_ = std::nullptr_t { };
      return input_;
    }

    char peek() const {
      return static_cast<char>(input_->peek());
    }
    char get() {
      return static_cast<char>(input_->get());
    }

    std::istream* input_;
    std::string token_;

    friend bool operator==(stream_token_iterator const&, stream_token_iterator const&);
  };

  bool operator==(stream_token_iterator const& lhs, stream_token_iterator const& rhs) {
    return
      lhs.token_.empty() && !lhs.input_ &&
      rhs.token_.empty() && !rhs.input_;
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

void test_tokenizer(std::string in, std::vector<std::string> expected) {
  SECTION(in) {
    auto input = std::istringstream(in);

    auto tok = stiX::tokenizer(input);
    auto e = expected.begin();
    for (auto token: tok) {
      if (e == expected.end())
        FAIL("Too many tokens");
      REQUIRE(token == *e);
      ++e;
    }
    if (e != expected.end())
      FAIL("Not enough tokens");
  }
}

TEST_CASE("tokenizer") {
  test_tokenizer("one", { "one" });
  test_tokenizer("one two three", {
    "one", " ", "two", " ", "three"
  });
  test_tokenizer("define(x, 1)", {
    "define",
    "(",
    "x",
    ",",
    " ",
    "1",
    ")"
  });
}
