#ifndef STICPP_FILE_OPEN_HPP
#define STICPP_FILE_OPEN_HPP

#include <string>
#include <fstream>

namespace stiX {
    std::ifstream file_opener(std::string const& filename);
    std::ofstream file_write_opener(std::string const& filename);
}

#endif //STICPP_FILE_OPEN_HPP
