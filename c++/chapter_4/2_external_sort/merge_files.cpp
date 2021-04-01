#include "merge_files.hpp"
#include <deque>
#include <fstream>

class merge_source {
public:
  explicit merge_source(std::istream& str)
    : stream_(&str) {
    next();
  }

  std::string const& line() const { return line_; }
  bool next() {
    std::getline(*stream_, line_);
    return stream_->good();
  }

private:
  std::istream* stream_;
  std::string line_;
};

bool operator <(const merge_source &a, const merge_source &b) {
  return a.line() < b.line();
}

void merge_all(std::ostream& out, std::deque<merge_source>& merge_sources);
void remove_all(std::vector<std::filesystem::path> const& working_files);

void merge_files(std::ostream& out, std::vector<std::filesystem::path> const& working_files) {
  auto file_sources = std::vector<std::ifstream> { };
  auto merge_sources = std::deque<merge_source> { };

  for (auto const& working_file : working_files)
    file_sources.emplace_back(working_file);

  for (auto& file_source: file_sources)
    merge_sources.emplace_back(file_source);

  merge_all(out, merge_sources);

  remove_all(working_files);
} // merge_files

void merge_all(std::ostream& out, std::deque<merge_source>& merge_sources) {
  while(!merge_sources.empty()) {
    std::sort(merge_sources.begin(), merge_sources.end());

    auto first = merge_sources.begin();
    out << first->line() << "\n";

    if (!first->next())
      merge_sources.pop_front();
  }
}

void remove_all(std::vector<std::filesystem::path> const& working_files) {
  for (auto const& working_file : working_files)
    std::filesystem::remove(working_file);
}
