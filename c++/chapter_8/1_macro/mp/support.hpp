#ifndef STICPP_SUPPORT_HPP
#define STICPP_SUPPORT_HPP

#include <string>
#include <vector>

namespace stiX {
  class token_stream;
  class token_seq;
}

namespace mp {
  void skip_whitespace(stiX::token_stream& tokens);

  stiX::token_seq parenthesised_sequence(stiX::token_stream& tokens);
  stiX::token_stream all_arguments(stiX::token_stream& tokens);

  bool is_next(stiX::token_stream& tokens, std::string_view expected);
  void check_next(stiX::token_stream& tokens, std::string_view expected);
  void expect_next(stiX::token_stream& tokens, std::string_view expected);

  bool not_reached(stiX::token_stream& tokens, std::string_view end_marker);

  std::string definition_name(stiX::token_stream& source);

  int argument_index(std::string const& index_tok);
  stiX::token_seq argument_substitution(
    stiX::token_stream& definition,
    std::vector<stiX::token_seq> const& arguments
  );
  stiX::token_seq next_argument(stiX::token_stream& tokens);
  std::vector<stiX::token_seq> gather_arguments(stiX::token_stream& source);
} // namespace mp

#endif //STICPP_SUPPORT_HPP
