#include "../../testlib/testlib.h"
#include "create.hpp"

TEST_CASE("Chapter 3 - archive create") {
  struct createArchiveTest {
    std::string const title;
    std::vector<stiX::input_file> const inputs;
    std::string const expected;
  };

  createArchiveTest tests[] = {
    {
      "no input files creates empty archive",
      { },
      ""
    },
    {
      "one zero-length input file",
      { { "nothing", 0 } },
      "-h- nothing 0\n"
    },
    {
      "two zero-length input files",
      {
        {"nothing", 0},
        {"empty", 0}
      },
      "-h- nothing 0\n-h- empty 0\n"
    }
  };

  for (auto t : tests) {
    DYNAMIC_SECTION(t.title) {
      std::ostringstream archiveOut;

      create_archive(t.inputs, archiveOut);

      auto archive = archiveOut.str();
      REQUIRE(archive == t.expected);
    }
  }
}