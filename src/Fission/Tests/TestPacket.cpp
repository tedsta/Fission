#include <UnitTest++/UnitTest++.h>

#include <array>
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

TEST(Packet_fromArray)
{
    std::array<unsigned char, 6> hello1({{'H', 'E', 'L', 'L', 'O', 0}});
    std::array<unsigned char, 6> hello2;

    Packet packet(hello1.data(), hello1.size());

    packet >> hello2;

    for (int i = 0; i < 6; i++)
        CHECK(hello1[i] == hello2[i]);
}

TEST(Packet_prepend)
{
    std::array<unsigned char, 6> hello1({{'H', 'E', 'L', 'L', 'O', 0}});
    std::array<unsigned char, 6> hello2;

    Packet packet;
    packet << int(42);
    packet.prepend(hello1.data(), hello1.size());

    int test;

    packet >> hello2;
    packet >> test;

    for (int i = 0; i < 6; i++)
        CHECK(hello1[i] == hello2[i]);
    CHECK(test == 42);
}

TEST(Packet_append)
{
    std::array<unsigned char, 6> hello1({{'H', 'E', 'L', 'L', 'O', 0}});
    std::array<unsigned char, 6> hello2;

    Packet packet;
    packet << int(42);
    packet.append(hello1.data(), hello1.size());

    int test;

    packet >> test;
    packet >> hello2;

    for (int i = 0; i < 6; i++)
        CHECK(hello1[i] == hello2[i]);
    CHECK(test == 42);
}
