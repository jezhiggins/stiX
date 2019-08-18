
#include "../../testlib/testlib.h"

#include "compress.h"

void testCompress(std::string input, std::string expected);

TEST_CASE("Chapter 2 - compress") {
    testCompress("", "");
    testCompress("1", "1");
    testCompress("11", "11");
    testCompress("111", "111");
    testCompress("1111", "~D1");
    testCompress("11111", "~E1");
    testCompress("111111", "~F1");
    testCompress("1111111", "~G1");
    testCompress("11111111", "~H1");
    testCompress("111111111", "~I1");

    testCompress("A1", "A1");
    testCompress("A11", "A11");
    testCompress("A111", "A111");
    testCompress("A1111", "A~D1");
    testCompress("A11111", "A~E1");
    testCompress("A111111", "A~F1");
    testCompress("A1111111", "A~G1");
    testCompress("A11111111", "A~H1");
    testCompress("A111111111", "A~I1");
    testCompress("1A", "1A");
    testCompress("11A", "11A");
    testCompress("111A", "111A");
    testCompress("1111A", "~D1A");
    testCompress("11111A", "~E1A");
    testCompress("111111A", "~F1A");
    testCompress("1111111A", "~G1A");
    testCompress("11111111A", "~H1A");
    testCompress("111111111A", "~I1A");
    testCompress("A1B", "A1B");
    testCompress("A11B", "A11B");
    testCompress("A111B", "A111B");
    testCompress("A1111B", "A~D1B");
    testCompress("A11111B", "A~E1B");
    testCompress("A111111B", "A~F1B");
    testCompress("A1111111B", "A~G1B");
    testCompress("A11111111B", "A~H1B");
    testCompress("A111111111B", "A~I1B");
    testCompress("A1B222222222C", "A1B~I2C");
    testCompress("A11B22222222C", "A11B~H2C");
    testCompress("A111B2222222C", "A111B~G2C");
    testCompress("A1111B222222C", "A~D1B~F2C");
    testCompress("A11111B22222C", "A~E1B~E2C");
    testCompress("A111111B2222C", "A~F1B~D2C");
    testCompress("A1111111B222C", "A~G1B222C");
    testCompress("A11111111B22C", "A~H1B22C");
    testCompress("A111111111B2C", "A~I1B2C");

    testCompress(std::string(25, 'a'), "~Ya");
    testCompress(std::string(26, 'a'), "~Za");
    testCompress(std::string(27, 'a'), "~Zaa");
    testCompress(std::string(28, 'a'), "~Zaaa");
    testCompress(std::string(29, 'a'), "~Zaaaa");
    testCompress(std::string(30, 'a'), "~Za~Da");
    testCompress(std::string(31, 'a'), "~Za~Ea");

    testCompress(std::string(51, 'a'), "~Za~Ya");
    testCompress(std::string(52, 'a'), "~Za~Za");
    testCompress(std::string(53, 'a'), "~Za~Zaa");
    testCompress(std::string(54, 'a'), "~Za~Zaaa");
    testCompress(std::string(55, 'a'), "~Za~Zaaaa");
    testCompress(std::string(56, 'a'), "~Za~Za~Da");
    testCompress(std::string(57, 'a'), "~Za~Za~Ea");

    testCompress("~", "~A~");
    testCompress("~1", "~A~1");
    testCompress("1~", "1~A~");
    testCompress("1~1", "1~A~1");
    testCompress("~~", "~B~");
    testCompress("~~~", "~C~");
    testCompress("~~~~", "~D~");
    testCompress(std::string(25, '~'), "~Y~");
    testCompress(std::string(26, '~'), "~Z~");
    testCompress(std::string(27, '~'), "~Z~~A~");
    testCompress(std::string(28, '~'), "~Z~~B~");
    testCompress(std::string(29, '~'), "~Z~~C~");
    testCompress(std::string(30, '~'), "~Z~~D~");
    testCompress(std::string(31, '~'), "~Z~~E~");
}

void testCompress(std::string input, std::string expected) {
    testFilter("compress", input, expected, stiX::compress);
}
