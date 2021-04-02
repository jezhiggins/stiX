#include "read_to_files.hpp"
#include "working_filepaths.hpp"
#include <fstream>
#include <iterator>
#include "../../lib/getline.hpp"
#include "merge_files.hpp"

using working_files_t = std::vector<std::filesystem::path>;
using lines_t = std::vector<std::string>;

lines_t read_lines(std::istream& in, int max_lines);
std::filesystem::path write_lines(lines_t const& lines);
void write_lines(std::filesystem::path const&, lines_t const& lines);
working_files_t merge_intermediates(working_files_t const& working_files);

auto const merge_order = 5;
auto const max_lines_to_read = 25000;

working_files_t read_to_files(std::istream& in) {
  auto working_files = working_files_t { };

  while (in) {
    while (in && working_files.size() < merge_order) {
      auto lines = read_lines(in, max_lines_to_read);

      std::sort(lines.begin(), lines.end());

      auto new_file = write_lines(lines);
      working_files.push_back(new_file);
    };

    if (in) // merge if there's still input to process
      working_files = merge_intermediates(working_files);
  }

  return working_files;
}

lines_t read_lines(std::istream& in, int max_lines) {
  auto lines = lines_t { };

  auto count = 0;
  while (in && count++ != max_lines) {
    auto line = stiX::getline(in);
    if (in)
      lines.emplace_back(line);
  }
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

working_files_t merge_intermediates(working_files_t const& working_files) {
  auto merge_filename = new_working_filepath();
  auto merge_file = std::ofstream{merge_filename};
  merge_files(merge_file, working_files);

  return { merge_filename };
}