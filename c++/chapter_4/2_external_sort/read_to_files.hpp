#ifndef STICPP_READ_TO_FILES_HPP
#define STICPP_READ_TO_FILES_HPP

#include <vector>
#include <filesystem>

std::vector<std::filesystem::path> read_to_files(std::istream& in);

void remove_all(std::vector<std::filesystem::path> &working_files);

#endif //STICPP_READ_TO_FILES_HPP
