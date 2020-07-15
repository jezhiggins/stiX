#include "../../testlib/testlib.h"
#include "create.hpp"

TEST_CASE("Chapter 3 - archive create") {
  SECTION("no input files creates empty archive") {
    std::ostringstream archiveOut;

    stiX::create_archive({ }, archiveOut);

    auto archive = archiveOut.str();
    REQUIRE(archive.empty());
  }

  SECTION("one zero-length input file") {
    std::ostringstream archiveOut;

    auto input = std::vector<stiX::input_file> { { "nothing", 0 } };
    stiX::create_archive(input, archiveOut);

    auto archive = archiveOut.str();
    REQUIRE(archive == "-h- nothing 0\n");
  }

  SECTION("two zero-length input files") {
    std::ostringstream archiveOut;

    auto input = std::vector<stiX::input_file> {
      {"nothing", 0},
      {"empty", 0}
    };
    stiX::create_archive(input, archiveOut);

    auto archive = archiveOut.str();
    REQUIRE(archive == "-h- nothing 0\n-h- empty 0\n");
  }
}