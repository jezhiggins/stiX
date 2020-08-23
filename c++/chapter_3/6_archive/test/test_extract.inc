#include "../extract.hpp"

void test_extract(
  std::string const& archive,
  std::vector<std::string> const& to_extract,
  std::string const& expected
);

TEST_CASE("Chapter 3 - archive extract mock") {
  SECTION("Empty archive") {
    test_extract("", { }, "");
  }
  SECTION("Archive with contents, but print nothing") {
    test_extract(
      archiveWithContents,
      { },
      ""
    );
  }
  SECTION("First file in archive") {
    test_extract(
      archiveWithContents,
      { "testFile1" },
      testFile1
    );
  }
  SECTION("Second file in archive") {
    test_extract(
      archiveWithContents,
      { "testFile2" },
      testFile2
    );
  }
  SECTION("Last file in archive") {
    test_extract(
      archiveWithContents,
      { "testFile3" },
      testFile3
    );
  }
}

void test_extract(
  std::string const& archive,
  std::vector<std::string> const& to_extract,
  std::string const& expected
) {
  auto archive_in = std::istringstream(archive);

  auto filename = std::string();
  auto out = std::ostringstream();

  auto mock_writer = [&filename, &out](std::string const& f) -> std::ostream& {
    filename = f;
    return out;
  };

  stiX::extract_files(archive_in, to_extract, mock_writer);

  if (to_extract.size())
    REQUIRE(filename == to_extract[0]);
  REQUIRE(out.str() == expected);
}

TEST_CASE("Chapter 3 - extract") {
  app_fixture("extract", "-x");
}