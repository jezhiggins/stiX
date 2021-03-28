#include "read_to_files.hpp"
#include "working_filepaths.hpp"
#include <fstream>
#include <iterator>
#include "../../lib/getline.hpp"

using working_files_t = std::vector<std::filesystem::path>;
using lines_t = std::vector<std::string>;

lines_t read_lines(std::istream& in, int max_lines);
std::filesystem::path write_lines(lines_t const& lines);
void write_lines(std::filesystem::path const&, lines_t const& lines);

auto const max_lines_to_read = 10000;

working_files_t read_to_files(std::istream& in) {
  auto working_files = std::vector < std::filesystem::path > {};

  while (in) {
    auto lines = read_lines(in, max_lines_to_read);

    std::sort(lines.begin(), lines.end());

    auto new_file = write_lines(lines);
    working_files.push_back(new_file);
  };

  return working_files;
}

lines_t read_lines(std::istream& in, int max_lines) {
  auto lines = std::vector<std::string> { };

  auto count = 0;
  while (in && count++ != max_lines)
    lines.emplace_back(stiX::getline(in));

  return lines;
} // read_lines

std::filesystem::path write_lines(lines_t const& lines) {
  auto temp_file = new_working_filepath();
  write_lines(temp_file, lines);
  return temp_file;
} // write_lines

void write_lines(std::filesystem::path const& filename, lines_t const& lines) {
  auto out = std::ofstream(filename);
  std::copy(
    lines.begin(),
    lines.end(),
    std::ostream_iterator<std::string>(out, "\n")
  );
} // write_lines
