#ifndef STICPP_DELETE_HPP
#define STICPP_DELETE_HPP

#include <iosfwd>
#include <vector>

namespace stiX {
  void delete_from_archive(
    std::istream& archive_in,
    std::vector<std::string> const& files_to_remove,
    std::ostream& archive_out
  );
} // delete_from_archive

#endif //STICPP_DELETE_HPP
