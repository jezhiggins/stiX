#include "../create.hpp"
#include "../archive.hpp"

std::string mock_file_1= "I am a test file\n";
std::string mock_file_2 = "I am another file\n";

std::map<std::string, std::string> mock_files = {
    { "testFile1", mock_file_1 },
    { "testFile2", mock_file_2 }
};

std::istringstream mock_file_contents(std::string const& name) {
  auto tf = mock_files.find(name);
  if (tf == mock_files.end())
    return std::istringstream();
  return std::istringstream(tf->second);
} // mock_file_contents

void file_fixture(std::string const& name);

void app_fixture(std::string const& name, std::string const& cmd);

TEST_CASE("Chapter 3 - archive create mock") {
  struct mock_create_test {
    std::string const title;
    std::vector<stiX::archive_file> const inputs;
    std::string const expected;
  };

  mock_create_test mocked_tests[] = {
    {
      "no input files creates empty archive",
      { },
      ""
    },
    {
      "one zero-length input file",
      { { "test.txt", 0 } },
      "-h- test.txt 0\n"
    },
    {
      "two zero-length input files",
      {
        {"test.txt", 0},
        {"empty", 0}
      },
      "-h- test.txt 0\n-h- empty 0\n"
    },
    {
      "one file, with contents",
      { { "testFile1", mock_file_1.size() } },
      "-h- testFile1 " + std::to_string(mock_file_1.size()) + "\n" + mock_file_1
    },
    {
      "two files, with contents",
      {
      { "testFile1", mock_file_1.size() },
      { "testFile2", mock_file_2.size() }
      },
      "-h- testFile1 " + std::to_string(mock_file_1.size()) + "\n" + mock_file_1 +
      "-h- testFile2 " + std::to_string(mock_file_2.size()) + "\n" + mock_file_2
    }
  };

  for (auto m : mocked_tests) {
    DYNAMIC_SECTION("Mocked " << m.title) {
      std::ostringstream archive_out;

      create_archive(m.inputs, archive_out, mock_file_contents);

      auto archive = archive_out.str();
      REQUIRE(archive == m.expected);
    }
  }
}

TEST_CASE("Chapter 3 - archive create") {
  file_fixture(
    "create",
    [](std::vector<stiX::archive_file> const& input, std::ostream& archive_out) {
      stiX::create_archive(input, archive_out, stiX::file_opener);
    }
  );
}

TEST_CASE("Chapter 3 - create") {
  app_fixture("create", "-c");
}

/////////////////////////////////////////////
void app_file_test(
  std::string const& title,
  fs::path const& directory,
  std::string const& cmd,
  std::string const& archive_file_name
);

void app_fixture(std::string const& name, std::string const& cmd) {
  auto fixture_dir = working_directory(name);

  for (auto& p : fs::directory_iterator(fixture_dir)) {
    auto title = fs::relative(p.path()).string();

    clean_output_directory(p);

    auto output_file = output_file_name(p.path());
    app_file_test(title, p.path(), cmd, output_file);
  }
} // app_fixture

void app_file_test(
  std::string const& title,
  fs::path const& directory,
  std::string const& cmd,
  std::string const& archive_file_name
) {
  DYNAMIC_SECTION("App " << title) {
    auto input_files = list_input_files(directory);

    auto arguments = std::vector {cmd, archive_file_name };
    for (auto i : input_files)
      arguments.push_back(i.name);

    {
      auto wd = working_directory(directory / "input");
      stiX::archive(arguments);
    }

    matches_expected(directory);
  }
} // app_file_test
