#include "catch.hpp"
#include "verify_with_samples.h"
#include "sample_strings.h"
#include "label_escape.h"


void verify_with_samples(std::string label, void (*verifyFn)(std::string)) {
    for (auto s : sample_strings()) {
        DYNAMIC_SECTION(label << "(\"" << escape(s) << "\")") {
            verifyFn(s);
        }
    }
}