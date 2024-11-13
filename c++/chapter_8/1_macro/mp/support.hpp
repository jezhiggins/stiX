#ifndef STICPP_SUPPORT_HPP
#define STICPP_SUPPORT_HPP

#include <string>
#include <vector>
#include <functional>

namespace stiX {
  class token_stream;
  class token_seq;
}

namespace mp {
  using token_sink = std::function<void(std::string const&)>;
  bool do_not_evaluate(
    std::string const& macro,
    stiX::token_stream& source,
    token_sink& sink);
  void skip_whitespace(stiX::token_stream& tokens);

  stiX::token_seq bracketed_sequence(
    stiX::token_stream& tokens,
    std::string_view opening,
    std::string_view closing);
  stiX::token_seq parenthesised_sequence(stiX::token_stream& tokens);
  void drop_bracketing(stiX::token_seq &tokens);
  stiX::token_stream all_arguments(stiX::token_stream& tokens);

  bool is_next(stiX::token_stream& tokens, std::string_view expected);
  void check_next(stiX::token_stream& tokens, std::string_view expected);
  void expect_next(stiX::token_stream& tokens, std::string_view expected);

  bool not_reached(stiX::token_stream& tokens, std::string_view end_marker);

  int argument_index(std::string const& index_tok);
  stiX::token_seq argument_substitution(
    stiX::token_stream& definition,
    std::vector<stiX::token_seq> const& arguments
  );
  stiX::token_seq next_argument(stiX::token_stream& tokens);
  std::vector<stiX::token_seq> gather_arguments(stiX::token_stream& source);

  std::pair<int, bool> int_arg(std::vector<std::string> const& args, size_t index, int def = 0);
} // namespace mp

#endif //STICPP_SUPPORT_HPP
