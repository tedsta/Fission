#include "Fission/Core/Packet.h"

namespace fsn
{
    Packet::Packet() : mStart(0)
    {
        //ctor
    }

    Packet::Packet(const unsigned char* data, std::size_t len) : mStart(0)
    {
        mStream.assign(data, data+len);
    }

    void Packet::prepend(const unsigned char* data, std::size_t len)
    {
        mStream.insert(mStream.begin(), data, data+len);
    }

    void Packet::append(const unsigned char* data, std::size_t len)
    {
        mStream.insert(mStream.end(), data, data+len);
    }

    void Packet::clear()
    {
        mStream.clear();
    }
}
