#include "../../testlib/testlib.hpp"

#include "pattern_matcher.hpp"
#include "matcher.hpp"
#include "char_seq.hpp"

TEST_CASE("Chapter 5 - character_sequence") {
  SECTION("empty sequence") {
    auto seq = stiX::character_sequence("");
    REQUIRE(seq.is_bol());
    REQUIRE(seq.is_eol());
  }
  SECTION("single character") {
    auto seq = stiX::character_sequence("a");
    REQUIRE(seq.is_bol());
    REQUIRE_FALSE(seq.is_eol());
    REQUIRE(*seq == 'a');

    seq.advance();
    REQUIRE_FALSE(seq.is_bol());
    REQUIRE(seq.is_eol());
  }
  SECTION("longer string") {
    auto seq = stiX::character_sequence("abcde");
    REQUIRE(seq.is_bol());
    REQUIRE_FALSE(seq.is_eol());

    REQUIRE(*seq == 'a');
    seq.advance();
    REQUIRE_FALSE(seq.is_bol());
    REQUIRE_FALSE(seq.is_eol());

    REQUIRE(*seq == 'b');
    seq.advance();
    REQUIRE_FALSE(seq.is_bol());
    REQUIRE_FALSE(seq.is_eol());

    REQUIRE(*seq == 'c');
    seq.advance();
    REQUIRE_FALSE(seq.is_bol());
    REQUIRE_FALSE(seq.is_eol());

    REQUIRE(*seq == 'd');
    seq.advance();
    REQUIRE_FALSE(seq.is_bol());
    REQUIRE_FALSE(seq.is_eol());

    REQUIRE(*seq == 'e');
    seq.advance();
    REQUIRE_FALSE(seq.is_bol());
    REQUIRE(seq.is_eol());
  }
}

TEST_CASE("Chapter 5 - find - single matcher") {
  using cs = stiX::character_sequence;

  SECTION("single character match") {
    auto m = stiX::make_matcher("a");
    REQUIRE(m.match(cs("a")));
    REQUIRE_FALSE(m.match(cs("b")));
  }
  SECTION("any character match") {
    auto m = stiX::make_matcher("?");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("A")));
    REQUIRE(m.match(cs("?")));
    REQUIRE(m.match(cs(" ")));
    REQUIRE_FALSE(m.match(cs("\n")));
  }
  SECTION("multi-character match") {
    auto m = stiX::make_matcher("abc");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("b")));
    REQUIRE(m.match(cs("c")));
    REQUIRE_FALSE(m.match(cs("v")));
  }
}

TEST_CASE("Chapter 5 - find - pattern matcher") {
  using cs = stiX::character_sequence;
  SECTION("simple single-char match") {
    auto p = stiX::compile_pattern("h");
    REQUIRE(p.size() == 1);
    REQUIRE(p.match("hello"));
    REQUIRE_FALSE(p.match("goodbye"));
    REQUIRE_FALSE(p.match(""));
  }
  SECTION("char sequence match") {
    auto p = stiX::compile_pattern("hello");
    REQUIRE(p.size() == 5);
    REQUIRE(p.match("hello"));
    REQUIRE(p.match("hhhhhhello"));
    REQUIRE(p.match("kellohelloyellow"));
    REQUIRE_FALSE(p.match("goodbye"));
    REQUIRE_FALSE(p.match(""));
  }
  SECTION("char sequence with wild card") {
    auto p = stiX::compile_pattern("he??o");
    REQUIRE(p.size() == 5);
    REQUIRE(p.match("hello"));
    REQUIRE(p.match("hellhello"));
    REQUIRE(p.match("xxxxheXXo"));
    REQUIRE_FALSE(p.match("goodbye"));
    REQUIRE_FALSE(p.match("hell"));
    REQUIRE_FALSE(p.match("hel"));
    REQUIRE_FALSE(p.match(""));
  }
}