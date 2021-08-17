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

stiX::matcher compile(const std::string& input) {
  auto cs = stiX::character_sequence(input);
  return stiX::make_matcher(cs);
}

TEST_CASE("Chapter 5 - find - single matcher") {
  using cs = stiX::character_sequence;

  SECTION("single character match") {
    auto m = compile("a");
    REQUIRE(m.match(cs("a")));
    REQUIRE_FALSE(m.match(cs("b")));
  }
  SECTION("any character match") {
    auto m = compile("?");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("A")));
    REQUIRE(m.match(cs("?")));
    REQUIRE(m.match(cs(" ")));
    REQUIRE_FALSE(m.match(cs("\n")));
  }
  SECTION("% start of line match") {
    auto m = compile("%");

    auto emptyseq = stiX::character_sequence("");
    REQUIRE(m.match(emptyseq));

    auto seq = stiX::character_sequence("abc");
    REQUIRE(m.match(seq));
    seq.advance();
    REQUIRE_FALSE(m.match(seq));
    seq.advance();
    REQUIRE_FALSE(m.match(seq));
  }
  SECTION("$ end of line match") {
    auto m = compile("$");

    auto emptyseq = stiX::character_sequence("");
    REQUIRE(m.match(emptyseq));

    auto seq = stiX::character_sequence("abc");
    REQUIRE_FALSE(m.match(seq));
    seq.advance();
    REQUIRE_FALSE(m.match(seq));
    seq.advance();
    REQUIRE_FALSE(m.match(seq));
    seq.advance();
    REQUIRE(m.match(seq));
  }
  SECTION("[abc] match") {
    auto m = compile("[abc]");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("b")));
    REQUIRE(m.match(cs("c")));
    REQUIRE_FALSE(m.match(cs("v")));
  }
  SECTION("[ab-] match") {
    auto m = compile("[ab-]");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("b")));
    REQUIRE(m.match(cs("-")));
    REQUIRE_FALSE(m.match(cs("v")));
  }
  SECTION("[-ab] match") {
    auto m = compile("[-ab]");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("b")));
    REQUIRE(m.match(cs("-")));
    REQUIRE_FALSE(m.match(cs("v")));
  }
  SECTION("[a-b] match") {
    auto m = compile("[a-b]");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("b")));
    REQUIRE_FALSE(m.match(cs("-")));
    REQUIRE_FALSE(m.match(cs("v")));
  }
  SECTION("[a-z] match") {
    auto m = compile("[a-z]");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("b")));
    REQUIRE(m.match(cs("c")));
    REQUIRE(m.match(cs("z")));
    REQUIRE_FALSE(m.match(cs("-")));
    REQUIRE_FALSE(m.match(cs("A")));
  }
  SECTION("[a-zA-Z] match") {
    auto m = compile("[a-zA-Z]");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("b")));
    REQUIRE(m.match(cs("c")));
    REQUIRE(m.match(cs("z")));
    REQUIRE(m.match(cs("A")));
    REQUIRE(m.match(cs("B")));
    REQUIRE(m.match(cs("C")));
    REQUIRE(m.match(cs("Z")));
    REQUIRE_FALSE(m.match(cs("-")));
    REQUIRE_FALSE(m.match(cs("0")));
  }
  SECTION("[@[@]] match") {
    auto m = compile("[@[@]]");
    REQUIRE(m.match(cs("[")));
    REQUIRE(m.match(cs("]")));
    REQUIRE_FALSE(m.match(cs("-")));
    REQUIRE_FALSE(m.match(cs("0")));
  }
  SECTION("[a-Z] match (invalid dash range)") {
    auto m = compile("[a-Z]");
    REQUIRE(m.match(cs("a")));
    REQUIRE(m.match(cs("-")));
    REQUIRE(m.match(cs("Z")));
    REQUIRE_FALSE(m.match(cs("0")));
    REQUIRE_FALSE(m.match(cs("b")));
  }
  SECTION("[!=-^] match") {
    auto m = compile("[!=-^]");
    REQUIRE(m.match(cs("!")));
    REQUIRE(m.match(cs("=")));
    REQUIRE(m.match(cs("-")));
    REQUIRE(m.match(cs("^")));
    REQUIRE_FALSE(m.match(cs("0")));
    REQUIRE_FALSE(m.match(cs("b")));
  }
  SECTION("[^a-z] match")
  {
    auto m = compile("[^a-z]");
    REQUIRE_FALSE(m.match(cs("a")));
    REQUIRE_FALSE(m.match(cs("b")));
    REQUIRE_FALSE(m.match(cs("c")));
    REQUIRE_FALSE(m.match(cs("z")));
    REQUIRE(m.match(cs("-")));
    REQUIRE(m.match(cs("A")));
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
  SECTION("character class match") {
    auto p = stiX::compile_pattern("[helo]");
    REQUIRE(p.size() == 1);
    REQUIRE(p.match("hello"));
    REQUIRE(p.match("hhhhhhello"));
    REQUIRE(p.match("kellohelloyellow"));
    REQUIRE(p.match("goodbye"));
    REQUIRE_FALSE(p.match(""));
  }
  SECTION("character and character class match") {
    auto p = stiX::compile_pattern("h[el][el][el]o");
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
  SECTION("% anchors to start of line") {
    SECTION("%hello") {
      auto p = stiX::compile_pattern("%hello");
      REQUIRE(p.size() == 6);
      REQUIRE(p.match("hello"));
      REQUIRE(p.match("hello friend"));
      REQUIRE_FALSE(p.match("hell"));
      REQUIRE_FALSE(p.match("hhhhhhello"));
      REQUIRE_FALSE(p.match("kellohelloyellow"));
      REQUIRE_FALSE(p.match("goodbye"));
      REQUIRE_FALSE(p.match(""));
    }
    SECTION("%[helo]") {
      auto p = stiX::compile_pattern("%[helo]");
      REQUIRE(p.size() == 2);
      REQUIRE(p.match("hello"));
      REQUIRE(p.match("hello friend"));
      REQUIRE(p.match("hell"));
      REQUIRE(p.match("hhhhhhello"));
      REQUIRE_FALSE(p.match("kellohelloyellow"));
      REQUIRE_FALSE(p.match("goodbye"));
      REQUIRE_FALSE(p.match(""));
    }
  }
  SECTION("% is only special at start of pattern") {
    auto p = stiX::compile_pattern("percent %");
    REQUIRE(p.size() == 9);
    REQUIRE(p.match("percent %"));
    REQUIRE(p.match("percent %!"));
    REQUIRE_FALSE(p.match("goodbye"));
    REQUIRE_FALSE(p.match(""));
  }
  SECTION("$ anchors to end of line") {
    SECTION("hello$") {
      auto p = stiX::compile_pattern("hello$");
      REQUIRE(p.size() == 6);
      REQUIRE(p.match("hello"));
      REQUIRE(p.match("oh hello"));
      REQUIRE_FALSE(p.match("hell"));
      REQUIRE_FALSE(p.match("hello friend"));
      REQUIRE_FALSE(p.match("kellohelloyellow"));
      REQUIRE_FALSE(p.match("goodbye"));
      REQUIRE_FALSE(p.match(""));
    }
    SECTION("[helo]$") {
      auto p = stiX::compile_pattern("[helo]$");
      REQUIRE(p.size() == 2);
      REQUIRE(p.match("hello"));
      REQUIRE(p.match("oh hello"));
      REQUIRE(p.match("hell"));
      REQUIRE_FALSE(p.match("hello friend"));
      REQUIRE_FALSE(p.match("kellohelloyellow"));
      REQUIRE(p.match("goodbye"));
      REQUIRE_FALSE(p.match(""));
    }
  }
  SECTION("$ is only special at end of pattern") {
    auto p = stiX::compile_pattern("what $ that");
    REQUIRE(p.size() == 11);
    REQUIRE(p.match("oh what $ that proot"));
    REQUIRE(p.match("what $ that"));
    REQUIRE_FALSE(p.match("goodbye"));
    REQUIRE_FALSE(p.match(""));
  }
  SECTION("%$ matches empty string") {
    auto p = stiX::compile_pattern("%$");
    REQUIRE(p.size() == 2);
    REQUIRE(p.match(""));
    REQUIRE_FALSE(p.match("nope"));
  }
  SECTION("char sequence anchored at both ends") {
    auto p = stiX::compile_pattern("%he??o$");
    REQUIRE(p.size() == 7);
    REQUIRE(p.match("hello"));
    REQUIRE(p.match("heppo"));
    REQUIRE_FALSE(p.match("hell"));
    REQUIRE_FALSE(p.match("hhhhhhello"));
    REQUIRE_FALSE(p.match("kellohelloyellow"));
    REQUIRE_FALSE(p.match("goodbye"));
    REQUIRE_FALSE(p.match(""));
  }
  SECTION("escape sequence") {
    auto escape_tests = std::vector<std::array<std::string, 2>> {
      { "@", "@" },
      { "@@", "@" },
      { "@%", "%" },
      { "@[", "[" },
      { "@t", "\t" },
      { "@n", "\n" }
    };

    for (auto escape_test : escape_tests) {
      auto escape = escape_test[0];
      auto expected = escape_test[1];
      DYNAMIC_SECTION(escape) {
        auto p = stiX::compile_pattern(escape);
        REQUIRE(p.match(expected));
      }
    }
  }
  SECTION("* by itself") {
    auto p = stiX::compile_pattern("*");
    REQUIRE(p.size() == 1);
    REQUIRE(p.match("a star *"));
    REQUIRE_FALSE(p.match("a hash #"));
    REQUIRE_FALSE(p.match(""));
  }
  SECTION("hel*o") {
    auto p = stiX::compile_pattern("hel*o");
    REQUIRE(p.size() == 4);
    REQUIRE(p.match("heo"));
    REQUIRE(p.match("helo"));
    REQUIRE(p.match("hello"));
    REQUIRE(p.match("hhhhhhello"));
    REQUIRE(p.match("hhhhhhellllllllo"));
    REQUIRE(p.match("kellohelloyellow"));
    REQUIRE_FALSE(p.match("hep"));
    REQUIRE_FALSE(p.match("help"));
    REQUIRE_FALSE(p.match("hellp"));
    REQUIRE_FALSE(p.match("hhhhhhellp"));
    REQUIRE_FALSE(p.match("hhhhhhellllllllp"));
    REQUIRE_FALSE(p.match("goodbye"));
    REQUIRE_FALSE(p.match(""));
  }
}
