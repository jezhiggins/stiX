#ifndef STICPP_MERGE_FILES_HPP
#define STICPP_MERGE_FILES_HPP

#include <iosfwd>
#include <vector>
#include <filesystem>

void merge_files(std::ostream& out, std::vector<std::filesystem::path> const& paths);

#endif //STICPP_MERGE_FILES_HPP
