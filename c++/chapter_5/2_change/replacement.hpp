#ifndef STICPP_REPLACEMENT_HPP
#define STICPP_REPLACEMENT_HPP

#include <string>
#include <vector>

namespace stiX {
  std::vector<std::string> prepare_replacement(std::string const& str);
  bool is_ditto(std::string const& str);
}

#endif //STICPP_REPLACEMENT_HPP
