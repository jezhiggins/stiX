#include "../../lib/chars.hpp"
#include <iosfwd>
#include <string>

namespace stiX {
  class stream_token_iterator {
  public:
    stream_token_iterator(std::istream& input) ;
    stream_token_iterator();

    std::string operator*();
    stream_token_iterator& operator++();

  private:
    void token();
    std::string next_token();

    bool input_available();

    char peek() const;
    char get();

    std::istream* input_;
    std::string token_;

    friend bool operator==(
      stream_token_iterator const&,
      stream_token_iterator const&);
  };

  bool operator==(
    stream_token_iterator const&,
    stream_token_iterator const&);

  class tokenizer {
  public:
    tokenizer(std::istream& input);

    stream_token_iterator begin() const;
    stream_token_iterator end() const;

  private:
    std::istream& input_;
  };
}
