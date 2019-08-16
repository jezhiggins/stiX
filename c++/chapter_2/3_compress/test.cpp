
#include "../../testlib/testlib.h"

#include "compress.h"

void testCompress(std::string input, std::string expected);

TEST_CASE("Chapter 2 - overstrike") {
    testCompress("", "");
    testCompress("1", "1");
    testCompress("11", "11");
    testCompress("111", "111");
    testCompress("1111", "1~D");
    testCompress("11111", "1~E");
    testCompress("111111", "1~F");
    testCompress("1111111", "1~G");
    testCompress("11111111", "1~H");
    testCompress("111111111", "1~I");
    testCompress("A1", "A1");
    testCompress("A11", "A11");
    testCompress("A111", "A111");
    testCompress("A1111", "A1~D");
    testCompress("A11111", "A1~E");
    testCompress("A111111", "A1~F");
    testCompress("A1111111", "A1~G");
    testCompress("A11111111", "A1~H");
    testCompress("A111111111", "A1~I");
    testCompress("1A", "1A");
    testCompress("11A", "11A");
    testCompress("111A", "111A");
    testCompress("1111A", "1~DA");
    testCompress("11111A", "1~EA");
    testCompress("111111A", "1~FA");
    testCompress("1111111A", "1~GA");
    testCompress("11111111A", "1~HA");
    testCompress("111111111A", "1~IA");
    testCompress("A1B", "A1B");
    testCompress("A11B", "A11B");
    testCompress("A111B", "A111B");
    testCompress("A1111B", "A1~DB");
    testCompress("A11111B", "A1~EB");
    testCompress("A111111B", "A1~FB");
    testCompress("A1111111B", "A1~GB");
    testCompress("A11111111B", "A1~HB");
    testCompress("A111111111B", "A1~IB");
    testCompress("A1B222222222C", "A1B2~IC");
    testCompress("A11B22222222C", "A11B2~HC");
    testCompress("A111B2222222C", "A111B2~GC");
    testCompress("A1111B222222C", "A1~DB2~FC");
    testCompress("A11111B22222C", "A1~EB2~EC");
    testCompress("A111111B2222C", "A1~FB2~DC");
    testCompress("A1111111B222C", "A1~GB222C");
    testCompress("A11111111B22C", "A1~HB22C");
    testCompress("A111111111B2C", "A1~IB2C");
}

void testCompress(std::string input, std::string expected) {
    testFilter("compress", input, expected, stiX::compress);
}
