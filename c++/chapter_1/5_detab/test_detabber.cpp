#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

#include "detab_impl.h"

TEST_CASE("Chapter 1 - detab - detabber", "next_tab_position") {
    for (size_t p = 0; p != 8; ++p)
        REQUIRE(stiX::next_tab_stop(p) == 8);

}