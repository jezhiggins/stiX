
#define CATCH_CONFIG_MAIN
#include "../../catch.hpp"

#include "copy.h"

void verifyCopyString(std::string input)
{
    std::istringstream is(input);
    std::ostringstream os;

    stiX::copy(is, os);

    REQUIRE( os.str() == input );
}

const std::string empty;
const std::string zero_length("");
const std::string very_short("1");
const std::string longer("Hello World");
const std::string longer_with_line_breaks("\tHello\n\t\tWorld!\n");

TEST_CASE( "Chapter 1 - copy", "[factorial]" ) {
    verifyCopyString(empty);
    verifyCopyString(zero_length);
    verifyCopyString(very_short);
    verifyCopyString(longer);
    verifyCopyString(longer_with_line_breaks);
}