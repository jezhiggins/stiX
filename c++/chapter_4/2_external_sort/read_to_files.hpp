#ifndef STICPP_READ_TO_FILES_HPP
#define STICPP_READ_TO_FILES_HPP

#include <vector>
#include <filesystem>

std::vector<std::filesystem::path> read_to_files(std::istream& in);

#endif //STICPP_READ_TO_FILES_HPP
