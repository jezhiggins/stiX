
#include "../../testlib/testlib.h"

#include "expand.h"

void testExpand(std::string input, std::string expected);

TEST_CASE("Chapter 2 - compress") {
    testExpand("", "");
    testExpand("1", "1");
    testExpand("11", "11");
    testExpand("111", "111");
    testExpand("~D1", "1111");
    testExpand("~E1", "11111");
    testExpand("~F1", "111111");
    testExpand("~G1", "1111111");
    testExpand("~H1", "11111111");
    testExpand("~I1", "111111111");

    testExpand("A1", "A1");
    testExpand("A11", "A11");
    testExpand("A111", "A111");
    testExpand("A~D1", "A1111");
    testExpand("A~E1", "A11111");
    testExpand("A~F1", "A111111");
    testExpand("A~G1", "A1111111");
    testExpand("A~H1", "A11111111");
    testExpand("A~I1", "A111111111");
    testExpand("1A", "1A");
    testExpand("11A", "11A");
    testExpand("111A", "111A");
    testExpand("~D1A", "1111A");
    testExpand("~E1A", "11111A");
    testExpand("~F1A", "111111A");
    testExpand("~G1A", "1111111A");
    testExpand("~H1A", "11111111A");
    testExpand("~I1A", "111111111A");
    testExpand("A1B", "A1B");
    testExpand("A11B", "A11B");
    testExpand("A111B", "A111B");
    testExpand("A~D1B", "A1111B");
    testExpand("A~E1B", "A11111B");
    testExpand("A~F1B", "A111111B");
    testExpand("A~G1B", "A1111111B");
    testExpand("A~H1B", "A11111111B");
    testExpand("A~I1B", "A111111111B");
    testExpand("A1B~I2C", "A1B222222222C");
    testExpand("A11B~H2C", "A11B22222222C");
    testExpand("A111B~G2C", "A111B2222222C");
    testExpand("A~D1B~F2C", "A1111B222222C");
    testExpand("A~E1B~E2C", "A11111B22222C");
    testExpand("A~F1B~D2C", "A111111B2222C");
    testExpand("A~G1B222C", "A1111111B222C");
    testExpand("A~H1B22C", "A11111111B22C");
    testExpand("A~I1B2C", "A111111111B2C");

    testExpand("~Ya", std::string(25, 'a'));
    testExpand("~Za", std::string(26, 'a'));
    testExpand("~Zaa", std::string(27, 'a'));
    testExpand("~Zaaa", std::string(28, 'a'));
    testExpand("~Zaaaa", std::string(29, 'a'));
    testExpand("~Za~Da", std::string(30, 'a'));
    testExpand("~Za~Ea", std::string(31, 'a'));

    testExpand("~Za~Ya", std::string(51, 'a'));
    testExpand("~Za~Za", std::string(52, 'a'));
    testExpand("~Za~Zaa", std::string(53, 'a'));
    testExpand("~Za~Zaaa", std::string(54, 'a'));
    testExpand("~Za~Zaaaa", std::string(55, 'a'));
    testExpand("~Za~Za~Da", std::string(56, 'a'));
    testExpand("~Za~Za~Ea", std::string(57, 'a'));

    testExpand("~A~", "~");
    testExpand("~A~1", "~1");
    testExpand("1~A~", "1~");
    testExpand("1~A~1", "1~1");
    testExpand("~B~", "~~");
    testExpand("~C~", "~~~");
    testExpand("~D~", "~~~~");
    testExpand("~Y~", std::string(25, '~'));
    testExpand("~Z~", std::string(26, '~'));
    testExpand("~Z~~A~", std::string(27, '~'));
    testExpand("~Z~~B~", std::string(28, '~'));
    testExpand("~Z~~C~", std::string(29, '~'));
    testExpand("~Z~~D~", std::string(30, '~'));
    testExpand("~Z~~E~", std::string(31, '~'));
}

void testExpand(std::string input, std::string expected) {
    testFilter("expand", input, expected, stiX::expand);
}
