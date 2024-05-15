#ifndef STICPP_COMMANDS_HPP
#define STICPP_COMMANDS_HPP

#include "command.hpp"
#include <initializer_list>

namespace stiX {
  class commands {
  public:
    commands(std::initializer_list<action> cs) :
      commands_(cs) {
    }

    void operator()(
      std::istream& in,
      std::ostream& out,
      edit_buffer& buffer,
      std::string& filename) const;

  private:
    std::vector<action> commands_;

    friend std::vector<action> const& command_list(commands const&);
  };
}

#endif //STICPP_COMMANDS_HPP
