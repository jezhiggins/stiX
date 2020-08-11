#include "../create.hpp"
#include "../archive.hpp"
#include "../../1_compare/compare.hpp"

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
  file_fixture("create");
}

TEST_CASE("Chapter 3 - create") {
  app_fixture("create", "-c");
}

/////////////////////////////////////////////
std::vector<stiX::archive_file> list_input_files(fs::path const& directory);
void clean_output_directory(fs::path const& directory);
fs::path output_file_name(fs::path const& directory);
void matches_expected(fs::path const& name);

void file_test(std::string const& title, fs::path const& directory);
void app_file_test(
  std::string const& title,
  fs::path const& directory,
  std::string const& cmd,
  std::string const& archive_file_name
);

void file_fixture(std::string const& name) {
  auto fixture_dir = working_directory(name);

  for (auto& p : fs::directory_iterator(fixture_dir)) {
    auto title = fs::relative(p.path()).string();

    clean_output_directory(p);

    file_test(title, p.path());
  }
} // file_fixture

void file_test(std::string const& title, fs::path const& directory) {
  DYNAMIC_SECTION("Fixture " << title) {
    auto input_files = list_input_files(directory);

    {
      auto archive_out = std::ofstream(output_file_name(directory));
      auto wd = working_directory(directory / "input");
      stiX::create_archive(input_files, archive_out, stiX::file_opener);
    }

    matches_expected(directory);
  }
} // file_test

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

std::vector<stiX::archive_file> list_input_files(fs::path const& directory) {
  auto files = std::vector<stiX::archive_file>();

  auto wd = working_directory(directory / "input");
  for(auto& p: fs::directory_iterator(wd)) {
    auto filepath = p.path();
    auto filename = fs::relative(filepath, wd).string();

    if (filename[0] == '.')
      continue;

    auto size = fs::file_size(filepath);

    files.emplace_back(stiX::archive_file { filename, size });
  }

  std::sort(files.begin(), files.end(),
            [](stiX::archive_file const& lhs, stiX::archive_file const& rhs) -> bool
            {
              return lhs.name < rhs.name;
            }
  );

  return files;
} // list_input_files

void clean_output_directory(fs::path const& directory) {
  auto output_dir = directory / "output";

  for (auto& p: fs::directory_iterator(output_dir)) {
    if (fs::relative(p, output_dir).string()[0] == '.')
        continue;
    fs::remove(p);
  }
} // clean_output_directory

fs::path output_file_name(fs::path const& directory) {
  return directory / "output" / "archive";
} // output_file_name

void matches_expected(fs::path const& name) {
  auto output_dir = name / "output";
  auto expected_dir = name / "expected";

  for(auto& p: fs::directory_iterator(output_dir)) {
    auto output_file = p.path();
    auto filename = fs::relative(output_file, output_dir).string();

    if (filename[0] == '.')
      continue;

    auto expected_file = expected_dir / filename;

    auto output_is = std::ifstream(output_file);
    auto expected_is = std::ifstream(expected_file);

    auto result = std::ostringstream();

    stiX::compare(
      output_file,
      output_is,
      expected_file,
      expected_is,
      result
    );


    INFO(result.str());
    REQUIRE(result.str().empty());
  }
} // matches_expected
