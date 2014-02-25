#include "Fission/Core/Packet.h"

namespace fsn
{
    Packet::Packet() : mStart(0)
    {
        //ctor
    }

    void Packet::prepend(const unsigned char* data, std::size_t len)
    {
        mStream.insert(mStream.begin(), data, &data[len-1]);
    }

    void Packet::append(const unsigned char* data, std::size_t len)
    {
        mStream.insert(mStream.end(), data, &data[len-1]);
    }

    void Packet::clear()
    {
        mStream.clear();
    }
}
