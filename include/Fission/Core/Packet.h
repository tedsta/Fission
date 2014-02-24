#ifndef FISSION_PACKET_H
#define FISSION_PACKET_H

#include <meta-serialization/serialize.h>

class Packet
{
    public:
        Packet();

        /// \brief Generic insertion operator
        template <typename T>
        Packet& operator<<(const T& data)
        {
            serialize(data, mStream);

            return *this;
        }

    private:
        StreamType mStream;
};

#endif // FISSION_PACKET_H
