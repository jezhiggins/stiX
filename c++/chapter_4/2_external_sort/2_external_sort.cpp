#include "read_to_files.hpp"
#include <iostream>
#include <deque>
#include <fstream>

class merge_source {
public:
  explicit merge_source(std::istream& str) : stream_(&str) {
    next();
  }

  bool good() const { return stream_->good(); }
  std::string const& line() const { return line_; }
  void next() { std::getline(*stream_, line_); }

private:
  std::istream* stream_;
  std::string line_;
};

bool operator <(const merge_source &a, const merge_source &b) {
  return a.line() < b.line();
}

int main() {
  auto working_files = read_to_files(std::cin);
  auto file_sources = std::vector<std::ifstream> { };
  auto merge_sources = std::deque<merge_source> { };

  for (auto const& working_file : working_files)
    file_sources.emplace_back(working_file);

  for (auto& file_source: file_sources) {
    merge_sources.emplace_back( file_source );
  }

  while(!merge_sources.empty()) {
    std::sort(merge_sources.begin(), merge_sources.end());

    auto first = merge_sources.begin();
    std::cout << first->line() << "\n";
    first->next();
    if (!first->good())
      merge_sources.pop_front();
  }

  for (auto const& working_file : working_files)
    std::filesystem::remove(working_file);
} // main

