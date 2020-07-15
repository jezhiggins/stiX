#include "../../testlib/testlib.h"
#include "create.hpp"

TEST_CASE("Chapter 3 - archive create") {
  SECTION("no input files creates empty archive") {
    std::ostringstream archiveOut;

    stiX::create_archive(archiveOut);

    auto archive = archiveOut.str();
    REQUIRE(archive.empty());
  }
}