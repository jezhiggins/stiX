#include "../delete.hpp"

void test_delete(
  std::string const& archive,
  std::vector<std::string> const& to_remove,
  std::string const& expected
);

TEST_CASE("Chapter 3 - archive delete mock") {
  SECTION("Empty archive") {
    test_delete(
      "",
      { "fruit" },
      ""
    );
  }
}

void test_delete(
  std::string const& archive,
  std::vector<std::string> const& to_remove,
  std::string const& expected
) {
  auto archive_in = std::istringstream(archive);
  auto out = std::ostringstream();

  stiX::delete_from_archive(archive_in, to_remove, out);

  REQUIRE(out.str() == expected);
} // test_delete