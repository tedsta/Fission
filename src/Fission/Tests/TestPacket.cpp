#include <UnitTest++/UnitTest++.h>

#include <memory>

#include "Fission/Core/Packet.h"

using namespace fsn;

TEST(Packet_insertExtractMultiple)
{
    Packet packet;

    int i1 = 42;
    char c1 = 'C';
    std::string str1 = "hello";
    int i2;
    char c2;
    std::string str2;

    packet << i1 << c1 << str1;
    packet >> i2 >> c2 >> str2;

    CHECK(i1 == i2);
    CHECK(c1 == c2);
    CHECK(str1 == str2);
}
