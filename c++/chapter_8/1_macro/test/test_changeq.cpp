#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "test.hpp"

TEST_CASE("changeq()") {
  auto good = std::vector<good_case> {
    { "changeq(<>)",
      "changeq(<>)define(<ACCEPT>,<define(<ACCEPT>,<Already accepted>)Accepted>)ACCEPT, ACCEPT",
      "Accepted, Already accepted" },
    { "changeq([])",
      "define(hello, helloworld)len(hello),len(`hello'),len([hello]) changeq([])len(hello),len(`hello'),len([hello])",
      "10,5,12 10,12,5"
    },
    { "changeq([])changeq()",
        "define(hello, helloworld)len(hello),len(`hello'),len([hello]) "
          "changeq([])len(hello),len(`hello'),len([hello]) "
          "changeq()len(hello),len(`hello'),len([hello])",
        "10,5,12 10,12,5 10,5,12"
    },
    { "changeq(#)",
      "define(hello, helloworld)len(hello),len(`hello'),len(#hello#) changeq(#)len(hello),len(`hello'),len(#hello#)",
      "10,5,12 10,12,5"
    },
    { "define(quote,#) changeq(quote)",
      "define(quote,#)define(hello, helloworld)len(hello),len(`hello'),len(#hello#) changeq(quote)len(hello),len(`hello'),len(#hello#)",
        "10,5,12 10,12,5"
    },
    { "changeq does nothing",
      "changeq len(`hello')",
      "changeq 5"
    }
  };
  auto warning = std::vector<warning_case> {
    { "too many args to changeq", "changeq(<>,99)", "", "Warning: excess arguments to `changeq' ignored\n" }
  };
  auto bad = std::vector<bad_case> {
    { "changeq(pants", "Expected )" }
  };

  build_good_tests(good);
  build_warning_tests(warning);
  build_bad_tests(bad);
}