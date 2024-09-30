#include "macro.hpp"
#include "../../lib/copy.hpp"

stiX::macro_processor::macro_processor() {
}

void stiX::macro_processor::process(
    std::istream& in,
    std::ostream& out
) {
  stiX::copy(in, out);
}