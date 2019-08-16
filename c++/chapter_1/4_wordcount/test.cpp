
#include "../../testlib/testlib.h"
#include "wordcount.h"

void verifyWordCount(std::string input);

TEST_CASE("Chapter 1 - wordcount") {
    verify_with_samples("wordcount", verifyWordCount);
}

void verifyWordCount(std::string input)
{
    const char delims[] = " \t\n";
    std::vector<char> v(input.begin(), input.end());
    v.push_back(0);

    char* s = strtok(&(v[0]), delims);
    size_t wordcount = 0;
    while (s != 0) {
        ++wordcount;
        s = strtok(0, delims);
    }

    std::istringstream is(input);
    auto count = stiX::wordcount(is);

    REQUIRE(count == wordcount);
}
