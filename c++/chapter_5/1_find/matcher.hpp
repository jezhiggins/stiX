#ifndef STICPP_MATCHER_HPP
#define STICPP_MATCHER_HPP

#include <string>
#include <functional>

namespace stiX {
  class character_sequence;
  using match_fn = std::function<bool(character_sequence const&)>;
  using match_fn_with_len = std::pair<match_fn, bool>;

  class matcher {
  public:
    bool match(character_sequence const& candidate) const;
    bool consumes() const;

  private:
    explicit matcher(match_fn_with_len fn);

    match_fn fn_;
    bool consume_;

    friend matcher make_matcher(stiX::character_sequence&);
  };

  matcher make_matcher(stiX::character_sequence& characters);
}

#endif //STICPP_MATCHER_HPP
