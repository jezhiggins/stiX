#include "../create.hpp"
#include "../archive.hpp"
#include "../../1_compare/compare.hpp"

namespace fs = std::filesystem;

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
class working_directory {
public:
  working_directory(std::string const& fixture) :
    pwd_(fs::current_path()),
    cwd_(fs::current_path() / "fixture" / fixture) {
    fs::current_path(cwd_);
  } // FileTestFixture

  operator fs::path() const {
    return cwd_;
  } // operation fs::path

  ~working_directory() {
    fs::current_path(pwd_);
  } // ~FileTestFixture

private:
  fs::path pwd_;
  fs::path cwd_;
}; // working_directory

std::vector<stiX::archive_file> list_input_files(fs::path const& directory);
fs::path output_file_name(fs::path const& directory);
fs::path expected_file_name(fs::path const& directory);
std::string matches_expected(fs::path const& name);

void file_test(std::string const& title, fs::path const& directory);
void app_file_test(
  std::string const& title,
  fs::path const& directory,
  std::string const& cmd,
  std::string const& output_file_name
);

void file_fixture(std::string const& name) {
  auto fixture_dir = working_directory(name);

  for (auto& p : fs::directory_iterator(fixture_dir)) {
    auto title = fs::relative(p.path()).string();

    auto output_file = output_file_name(p.path());
    if (fs::exists(output_file))
      fs::remove(output_file);


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

    auto result = matches_expected(directory);

    INFO(result);
    REQUIRE(result.empty());
  }
} // file_test

void app_fixture(std::string const& name, std::string const& cmd) {
  auto fixture_dir = working_directory(name);

  for (auto& p : fs::directory_iterator(fixture_dir)) {
    auto title = fs::relative(p.path()).string();

    auto output_file = output_file_name(p.path());
    if (fs::exists(output_file))
      fs::remove(output_file);

    app_file_test(title, p.path(), cmd, output_file);
  }
} // app_fixture

void app_file_test(
  std::string const& title,
  fs::path const& directory,
  std::string const& cmd,
  std::string const& output_file_name
) {
  DYNAMIC_SECTION("App " << title) {
    auto input_files = list_input_files(directory);

    auto arguments = std::vector { cmd, output_file_name };
    for (auto i : input_files)
      arguments.push_back(i.name);

    {
      auto wd = working_directory(directory / "input");
      stiX::archive(arguments);
    }

    auto result = matches_expected(directory);

    INFO(result);
    REQUIRE(result.empty());
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

fs::path output_file_name(fs::path const& directory) {
  return directory / "output" / "result";
} // output_file_name

fs::path expected_file_name(fs::path const& directory) {
  return directory / "expected" / "result";;
} // expected_file_name

std::string matches_expected(fs::path const& name) {
  auto output = output_file_name(name);
  auto expected = expected_file_name(name);

  auto output_is = std::ifstream(output);
  auto expected_is = std::ifstream(expected);

  auto result = std::ostringstream();

  stiX::compare(
    output,
    output_is,
    expected,
    expected_is,
    result
  );

  return result.str();
} // matches_expected
