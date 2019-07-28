#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

#include "detab_impl.h"

TEST_CASE("Chapter 1 - detab - detabber", "next_tab_position") {
    for (size_t p = 0; p != stiX::tabSize; ++p)
        REQUIRE(stiX::next_tab_stop(p) == stiX::tabSize);
    for (size_t p = stiX::tabSize; p != (stiX::tabSize * 2); ++p)
        REQUIRE(stiX::next_tab_stop(p) == (stiX::tabSize * 2));

    for (size_t s = 0; s <= 10; ++s) {
        const auto p = (s * stiX::tabSize) + (stiX::tabSize / 2);
        REQUIRE(stiX::next_tab_stop(p) == ((s+1) * stiX::tabSize));
    }
}