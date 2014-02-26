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
            Packet(const unsigned char* data, std::size_t len);

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

            // Dealing with other packets

            /// \brief Append another packet onto this packet
            Packet& operator<<(const Packet& other)
            {
                append(other.getData(), other.getDataSize());
                return *this;
            }

            /// \brief Prepend this packet onto another packet
            void operator>>(Packet& other)
            {
                other.prepend(getData(), getDataSize());
            }

            // Other manipulation functions

            /// \brief Prepend a regular array onto this packet
            void prepend(const unsigned char* data, std::size_t len);

            /// \brief Append a regular array onto this packet
            void append(const unsigned char* data, std::size_t len);

            /// \brief Clear this packet
            void clear();

            /// \brief Get this packet's raw data
            const unsigned char* getData() const {return mStream.data();}

            /// \brief Get the length of this packet
            std::size_t getDataSize() const {return mStream.size();}

        private:
            StreamType mStream;
            std::size_t mStart;
    };
}


#endif // FISSION_PACKET_H
