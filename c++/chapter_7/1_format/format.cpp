#include "format.hpp"

#include "../../lib/getline.hpp"

#include <sstream>
#include <string>

using namespace std::string_literals;

namespace {
  auto constinit eof = std::char_traits<char>::eof();
  auto constinit null = std::string_view { };
  auto constinit space = std::string_view { " " };
}

class buffer {
public:
  buffer(std::ostream& out) : out_(out), maxlen_(60) { }
  ~buffer() {
    if (buffer_.length() != 0)
      flush();
  }

  buffer& operator<<(std::string const& s) {
    buffer_ += s;
    flush_if_wraps();
    return *this;
  }
  buffer& operator<<(std::string_view const& s) {
    buffer_ += s;
    flush_if_wraps();
    return *this;
  }

private:
  void flush_if_wraps() {
    while (buffer_.length() > maxlen_) {
      auto break_at = buffer_.rfind(' ', maxlen_);
      out_ << buffer_.substr(0, break_at) << '\n';
      buffer_ = buffer_.substr(break_at + 1);
    }
  }

  void flush() {
    out_ << buffer_ << '\n';
    buffer_.clear();
  }

  std::string buffer_;
  std::ostream& out_;
  size_t maxlen_;
};

void stiX::formatter::operator()(std::istream &in, std::ostream &out) {
  auto separator = null;
  auto buf = buffer { out };
  while (in.peek() != eof) {
    auto s = getline(in);

    buf << separator << s;

    separator = space;
  }
}
