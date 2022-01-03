#ifndef STICPP_REPLACEMENT_HPP
#define STICPP_REPLACEMENT_HPP

#include <string>
#include <utility>
#include <vector>

namespace stiX {
  class replacement {
  public:
    replacement(std::vector<std::string> r)
      : replacements_(std::move(r))
    {
    }

    std::string apply(std::string_view match) const;
    void apply(std::string_view match, std::ostream& out) const;

  private:
    std::vector<std::string> replacements_;

    friend struct replacement_test_fixture;
  };

  replacement prepare_replacement(std::string const& str);
}

#endif //STICPP_REPLACEMENT_HPP
