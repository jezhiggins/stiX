#include "../../testlib/testlib.hpp"
#include "../../lib/getline.hpp"

#include <sstream>
#include <string>

namespace stiX {
  auto constinit eof = std::char_traits<char>::eof();

  class formatter {
  public:
    void operator()(std::istream &in, std::ostream &out) {
      while (in.peek() != eof) {
        auto s = getline(in);
        out << s << '\n';
      }
    }
  };
}

TEST_CASE("unformatted text") {
  SECTION("short line") {
    auto in = std::istringstream { };
    in.str("hello\n");

    auto out = std::ostringstream { };
    auto formatter = stiX::formatter { };

    formatter(in, out);

    auto output = out.str();
    REQUIRE(output == "hello\n");
  }
}