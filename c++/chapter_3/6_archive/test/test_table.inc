#include "../table.hpp"

void test_table(
  std::string const& archive,
  std::string const& expected
) {
  auto archive_in = std::istringstream(archive);
  auto out = std::ostringstream();

  stiX::table_archive(archive_in, out);

  REQUIRE(out.str() == expected);
} // test_table

TEST_CASE("Chapter 3 - archive table") {
  SECTION("Empty archive") {
    test_table("", "");
  }

  SECTION("One zero length file") {
    test_table(
      "-h- nothing 0\n",
      "nothing\n"
    );
  }
}