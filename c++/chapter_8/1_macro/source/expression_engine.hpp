#ifndef STICPP_EXPRESSION_ENGINE_HPP
#define STICPP_EXPRESSION_ENGINE_HPP

#include "token_seq.hpp"
#include <vector>

namespace stiX {
  std::pair<int, bool> evaluate_expression(token_seq const& expression);
  std::pair<int, bool> evaluate_expression(std::vector<std::string> const& expression);
}
#endif //STICPP_EXPRESSION_ENGINE_HPP
