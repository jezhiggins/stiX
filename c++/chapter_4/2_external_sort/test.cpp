#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"

#include "working_filepaths.hpp"

TEST_CASE("Chapter 4 - temporary file names") {
  SECTION("unique temporary file names")
  {
    auto const how_many = 10;
    auto names = std::set<std::string> { };

    for (int i = 0; i != how_many; ++i)
      names.insert(new_working_filepath());

    REQUIRE(names.size() == how_many);
  }
}