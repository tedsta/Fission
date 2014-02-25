#ifndef FISSION_PACKET_H
#define FISSION_PACKET_H

#include <iostream>

#include <meta-serialization/serialize.h>

namespace fsn
{
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

            /// \brief Generic extraction operator
            template <typename T>
            Packet& operator>>(T& data)
            {
                data = deserialize<T>(mStream, &mStart);

                return *this;
            }

        private:
            StreamType mStream;
            std::size_t mStart;
    };
}


#endif // FISSION_PACKET_H
