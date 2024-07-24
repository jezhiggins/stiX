#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"
#include "format_functions.hpp"

TEST_CASE("fill line") {
  REQUIRE(stiX::fill_line("hello world", 12) == "hello  world");
  REQUIRE(stiX::fill_line("big fat pig", 12) == "big  fat pig");
  REQUIRE(stiX::fill_line("big fat pig", 13) == "big  fat  pig");
  REQUIRE(stiX::fill_line("big fat pig", 14) == "big   fat  pig");
  REQUIRE(stiX::fill_line("big fat pig", 15) == "big   fat   pig");
  REQUIRE(stiX::fill_line("big fat pig", 16) == "big    fat   pig");
}

TEST_CASE("centre line") {
  REQUIRE(stiX::centre_line("hello", 9) == "  hello");
  REQUIRE(stiX::centre_line("hello", 8) == " hello");
  REQUIRE(stiX::centre_line("hello", 10) == "  hello");
  REQUIRE(stiX::centre_line("hello", 5) == "hello");
}

TEST_CASE("underline") {
  REQUIRE(stiX::underline("hello") == "\e[4mhello\e[0m");
  REQUIRE(stiX::underline("hello world") == "\e[4mhello\e[0m \e[4mworld\e[0m");
  REQUIRE(stiX::underline("  hello world  ") == "  \e[4mhello\e[0m \e[4mworld\e[0m  ");
  REQUIRE(stiX::underline("     ") == "     ");
}

TEST_CASE("tokenise & width") {
  SECTION("unformatted text") {
    auto toks = stiX::split_into_words("\e[4mhello\e[0m \e[4mworld!\e[0m");
    auto expected = std::vector<stiX::word_width>{{"\e[4mhello\e[0m",  5},
                                                  {"\e[4mworld!\e[0m", 6}};

    REQUIRE(toks.size() == expected.size());
    for (auto i = 0; i != toks.size(); ++i) {
      REQUIRE(toks[i].word == expected[i].word);
      REQUIRE(toks[i].width == expected[i].width);
    }
  }
  SECTION("underlined text") {
    auto toks = stiX::split_into_words("hello everybody in the world!");
    auto expected = std::vector<stiX::word_width>{{"hello",  5},
                                                  {"everybody", 9},
                                                  {"in", 2},
                                                  {"the", 3},
                                                  {"world!", 6}};

    REQUIRE(toks.size() == expected.size());
    for (auto i = 0; i != toks.size(); ++i) {
      REQUIRE(toks[i].word == expected[i].word);
      REQUIRE(toks[i].width == expected[i].width);
    }
  }
}
