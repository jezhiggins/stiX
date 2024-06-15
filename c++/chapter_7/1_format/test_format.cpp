#include "../../testlib/testlib.hpp"

#include "format.hpp"

TEST_CASE("unformatted text") {
  SECTION("short line") {
    auto in = std::istringstream { };
    in.str("hello\n");

    auto out = std::ostringstream { };
    auto formatter = stiX::formatter { };

    formatter(in, out);

    auto output = out.str();
    REQUIRE(output == "hello\n");
  }
}