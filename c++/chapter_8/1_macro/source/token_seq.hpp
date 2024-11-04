#ifndef STICPP_TOKEN_SEQ_HPP
#define STICPP_TOKEN_SEQ_HPP

#include <deque>
#include <string>

namespace stiX {
  class token_seq {
  public:
    using const_iterator = std::deque<std::string>::const_iterator;
    using value_type = std::string;

    token_seq() = default;
    token_seq(std::initializer_list<std::string> toks) : seq_(toks) { }

    const_iterator begin() const { return seq_.begin(); }
    const_iterator end() const { return seq_.end(); }

    bool empty() const { return seq_.empty(); }

    std::string const& front() const { return seq_.front(); }

    void pop_front() { seq_.pop_front(); }
    void pop_back() { seq_.pop_back(); }

    void push_front(std::string const& tok) { seq_.push_front(tok); }
    void push_back(std::string const& tok) { seq_.push_back(tok); }

    token_seq& operator+=(std::string const& tok);
    token_seq& operator+=(std::string&& tok);
    token_seq& operator+=(token_seq&& tok);

  private:
    std::deque<std::string> seq_;
  };
}

#endif //STICPP_TOKEN_SEQ_HPP
