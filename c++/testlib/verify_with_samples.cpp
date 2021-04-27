#include "catch.hpp"
#include "verify_with_samples.hpp"
#include "sample_strings.hpp"
#include "label_escape.hpp"


void verify_with_samples(std::string label, void (*verifyFn)(std::string)) {
    for (auto s : sample_strings()) {
        DYNAMIC_SECTION(label << "(\"" << escape(s) << "\")") {
            verifyFn(s);
        }
    }
}