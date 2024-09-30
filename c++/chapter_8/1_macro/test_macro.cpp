#include "../../testlib/testlib.hpp"
#include "macro.hpp"

TEST_CASE("macro") {
  SECTION("pass through") {
    auto macro = stiX::macro_processor { };

    auto in = std::istringstream { "nothing going on here" };
    auto out = std::ostringstream { };

    macro.process(in, out);

    REQUIRE(out.str() == "nothing going on here");
  }
}