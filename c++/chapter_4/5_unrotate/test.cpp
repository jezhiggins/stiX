#include "../../testlib/testlib.hpp"
#include "unrotate.hpp"

void testUnrotateLine(std::string const& input, std::string const& expected);

TEST_CASE("Chapter 4 - unrotate") {
  SECTION("empty input") {
    testUnrotateLine("", "");
  }

  SECTION("no marker") {
    testUnrotateLine("Hello", "Hello");
  }

  SECTION("marker at end") {
    testUnrotateLine(
      "This is a test.$",
      "                     This is a test."
    );
  }
  SECTION("marker after first word") {
    testUnrotateLine(
      "test.$This is a",
      "          This is a  test."
    );
  }
  SECTION("marker in middle") {
    testUnrotateLine(
      "a test.$This is",
      "            This is  a test."
    );
  }
  SECTION("line is too long, wrap left") {
    testUnrotateLine(
      "test.$Watch out! Much girthier",
      " out! Much girthier  test.         Watch"
    );
  }

  SECTION("line is too long, wrap right") {
    testUnrotateLine(
      "out! Much girthier test.$Watch",
      "test.         Watch  out! Much girthier"
    );
  }

  SECTION("line is far too long, wrap right and get truncated") {
    testUnrotateLine(
      "out! Much, much, much girthier test.$Watch",
      "irthier test. Watch  out! Much, much, mu"
    );
  }
  SECTION("line is far too long, wrap left and get truncated") {
    testUnrotateLine(
      "test.$Watch out! Much, much, much girthier",
      "much, much girthier  test. Watch out! Mu"
    );
  }
  SECTION("line is far, far too long, wrap right and get truncated") {
    testUnrotateLine(
      "out! Much, much, much and so very excessively girthier test.$Watch",
      "irthier test. Watch  out! Much, much, mu"
    );
  }
  SECTION("line is far, far too long, wrap left and get truncated") {
    testUnrotateLine(
      "test.$Watch out! Much, and so very excessively much, much girthier",
      "much, much girthier  test. Watch out! Mu"
    );
  }
  SECTION("line is far too long at both ends") {
    testUnrotateLine(
      "Much, much, much girthier test.$Watch out! This line is excessive.",
      " line is excessive.  Much, much, much gi"
    );
  }
  SECTION("line is far too long at both ends") {
    testUnrotateLine(
      "Much, much, much girthier test.$Watch out! This line is excessive.",
      " line is excessive.  Much, much, much gi"
    );
  }
  SECTION("line is far, far too long at both ends") {
    testUnrotateLine(
      "Much, much, much girthier test. But this chunk will be trimmed.$Watch out! There's stuff you'll never see. This line is excessive.",
      " line is excessive.  Much, much, much gi"
    );
  }
}

void testUnrotateLine(std::string const& input, std::string const& expected) {
  auto result = stiX::unrotate_line(input, 40, '$');
  REQUIRE(result == expected);
}
