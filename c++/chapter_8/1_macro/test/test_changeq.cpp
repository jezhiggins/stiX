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
    }

  };
  auto bad = std::vector<bad_case> {
    { "changeq(pants", "Expected )" }
  };

  build_good_tests(good);
  build_bad_tests(bad);
}