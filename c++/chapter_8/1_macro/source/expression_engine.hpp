#ifndef STICPP_EXPRESSION_ENGINE_HPP
#define STICPP_EXPRESSION_ENGINE_HPP

#include <vector>
#include <string>

namespace stiX {
  std::pair<int, bool> evaluate(std::vector<std::string> expression);
}
#endif //STICPP_EXPRESSION_ENGINE_HPP
