#include "../../testlib/testlib.hpp"
#include "macro.hpp"

TEST_CASE("macro") {
  SECTION("pass through") {
    auto in = std::istringstream { "nothing going on here\nat all" };
    auto out = std::ostringstream { };

    stiX::macro_process(in, out);

    REQUIRE(out.str() == "nothing going on here\nat all");
  }

  SECTION("identify a definition") {
    auto in = std::istringstream { "nothing going on here\ndefine(x, y)\nat all" };
    auto out = std::ostringstream { };

    stiX::macro_process(in, out);

    REQUIRE(out.str() == "nothing going on here\n\nat all");
  }
}