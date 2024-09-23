#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"
using namespace std::string_literals;

namespace stiX {
  class tokenizer {
  public:
    tokenizer(std::istream& input) :
      input_(input) {
    }

    std::string begin() {
      return "one";
    }

  private:
    std::istream& input_;
  };
}

TEST_CASE("tokenizer") {
  auto input = std::istringstream("one");

  auto tok = stiX::tokenizer(input);
  REQUIRE(tok.begin() == "one"s);
}