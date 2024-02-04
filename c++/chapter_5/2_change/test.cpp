#include "../../testlib/testlib.hpp"

#include "../../lib/regex/change.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include "../../lib/regex/replacement.hpp"

TEST_CASE("Chapter 5 - change - pattern match location") {
  SECTION("no match") {
    auto p = stiX::compile_pattern("p");

    auto r = p.find("hello");
    REQUIRE_FALSE(r.match);
    REQUIRE(r.from == std::string::npos);
    REQUIRE(r.from == r.to);
  }

  SECTION("single-char match at start of line") {
    auto p = stiX::compile_pattern("h");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 1);
  }

  SECTION("single-char match at end of line") {
    auto p = stiX::compile_pattern("o");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 4);
    REQUIRE(r.to == 5);
  }

  SECTION("match at start of line") {
    auto p = stiX::compile_pattern("%hell");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 4);
  }

  SECTION("match at end of line") {
    auto p = stiX::compile_pattern("ello$");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 1);
    REQUIRE(r.to == 5);
  }

  SECTION("match only end of line") {
    auto p = stiX::compile_pattern("$");

    auto r = p.find("");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 0);

    auto q = p.find("help");
    REQUIRE(q.match);
    REQUIRE(q.from == 4);
    REQUIRE(q.to == 4);
  }

  SECTION("match only start of line") {
    auto p = stiX::compile_pattern("%");

    auto r = p.find("");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 0);

    auto q = p.find("help");
    REQUIRE(q.match);
    REQUIRE(q.from == 0);
    REQUIRE(q.to == 0);
  }

  SECTION("match in middle of line") {
    auto p = stiX::compile_pattern("ell");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 1);
    REQUIRE(r.to == 4);
  }

  SECTION("wildcard match at start of line") {
    auto p = stiX::compile_pattern("hel*");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 4);
  }

  SECTION("wildcard match at end of line") {
    auto p = stiX::compile_pattern("el*o");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 1);
    REQUIRE(r.to == 5);
  }

  SECTION("multiple wildcard match") {
    auto p = stiX::compile_pattern("h*e*l*o*");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 5);
  }

  SECTION("whole line match") {
    auto p = stiX::compile_pattern("%?*$");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 5);
  }

  SECTION("empty line match") {
    auto p = stiX::compile_pattern("%$");

    auto r = p.find("");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 0);
  }
}

namespace stiX {
  struct replacement_test_fixture {
    static std::vector<std::string> unwrap(stiX::replacement r) {
      return r.replacements_;
    }
  };
}

std::vector<std::string> prepare(std::string s) {
  using rtf = stiX::replacement_test_fixture;
  return rtf::unwrap(stiX::prepare_replacement(s));
}

TEST_CASE("Chapter 5 - prepare replacement")
{
  auto ditto = std::string { 1, '\0' };

  SECTION("replace with nothing") {
    auto replacement = prepare("");
    REQUIRE(0 == replacement.size());
  }

  SECTION("simple replacement") {
    auto strs = std::vector {
      "hello",
      "fruit"
    };

    for (auto& str : strs) {
      auto replacement = prepare(str);
      REQUIRE(1 == replacement.size());
      REQUIRE(str == replacement.front());
    }
  }

  SECTION("with escapes") {
    auto strs = std::vector<std::pair<std::string, std::string>> {
      { "@h@e@l@l@o", "hello" },
      { "hi@tthere", "hi\tthere" },
      { "@hello@", "hello@" }
    };

    for (auto [str, expected] : strs) {
      auto replacement = prepare(str);
      REQUIRE(1 == replacement.size());
      REQUIRE(expected == replacement.front());
    }
  }

  SECTION("dup at start") {
    auto replacement = prepare("&!?!?!");
    REQUIRE(2 == replacement.size());
    REQUIRE(ditto == replacement.front());
    REQUIRE("!?!?!" == replacement.back());
  }
  SECTION("dup at end") {
    auto replacement = prepare("-->&");
    REQUIRE(2 == replacement.size());
    REQUIRE("-->" == replacement.front());
    REQUIRE(ditto == replacement.back());
  }
  SECTION("dup in middle") {
    auto replacement = prepare("(&)");
    REQUIRE(3 == replacement.size());
    REQUIRE("(" == replacement[0]);
    REQUIRE(ditto == replacement[1]);
    REQUIRE(")" == replacement[2]);
  }
  SECTION("dup at beginning, middle, and end") {
    auto replacement = prepare("& ->@& (&) @&<- &");
    REQUIRE(5 == replacement.size());
    REQUIRE(ditto == replacement[0]);
    REQUIRE(" ->& (" == replacement[1]);
    REQUIRE(ditto == replacement[2]);
    REQUIRE(") &<- " == replacement[3]);
    REQUIRE(ditto == replacement[4]);
  }
}

void testChange(
  std::string const& input,
  std::string const& expected,
  std::string const& pattern,
  std::string const& replacement
);

TEST_CASE("Chapter 5 - change") {
  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "hello world\nsit down\nhave a biscuit\n",
    "123",
    "");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "hello world\nsit down\nhave a digestive\n",
    "biscuit",
    "digestive");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "hll wrld\nst dwn\nhv  bsct\n",
    "[aeiou]",
    "");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "Hello world\nsit down\nHave a biscuit\n",
    "%h",
    "H");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "hello world!\nsit down!\nhave a biscuit!\n",
    "$",
    "!");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "--- hello world\n--- sit down\n--- have a biscuit\n",
    "%",
    "--- ");

  testChange(
    "12345\n",
    "-1-2-3-4-5-\n",
    "a*",
    "-");

  testChange(
    "abc\n",
    "-b-c-\n",
    "a*",
    "-");

  testChange(
    "abc\n",
    "(a)bc\n",
    "a",
    "(&)");

  testChange(
    "abc\n",
    "(a)(b)(c)\n",
    "?",
    "(&)");
}

void testChange(
  std::string const& input,
  std::string const& expected,
  std::string const& pattern,
  std::string const& replacement
) {
  testFilter("change", input, expected,
             [pattern, replacement](std::istream& in, std::ostream& out) {
               stiX::change(in, out, pattern, replacement);
             }
  );
}