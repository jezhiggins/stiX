#include "format.hpp"

#include "../../lib/getline.hpp"

#include <sstream>
#include <string>

using namespace std::string_literals;

namespace {
  auto
  constinit eof = std::char_traits<char>::eof();
  auto
  constinit null = std::string_view{};
  auto
  constinit space = std::string_view{" "};

  class screen_formatter {
  public:
    screen_formatter(std::istream& in, std::ostream &out) :
      in_(in),
      out_(out),
      maxlen_(60) {
    }
    ~screen_formatter() {
      if (buffer_.length() != 0)
        flush();
    }

    void format() {
      auto separator = null;

      while (in_.peek() != eof) {
        auto line = stiX::getline(in_);

        buffer_ += separator;
        buffer_ += line;
        flush_if_wraps();

        separator = space;
      }
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

    std::istream& in_;
    std::ostream& out_;
    size_t maxlen_;
    std::string buffer_;
  };
}

void stiX::format(std::istream &in, std::ostream &out) {
  auto formatter = screen_formatter { in, out };
  formatter.format();
}
