#ifndef COMPONENT_H
#define COMPONENT_H

#include <bitset>

#include <SFML/Network/Packet.hpp>

#include <Fission/Core/config.h>
#include <Fission/Core/RefCounted.h>

// Type bits for components
typedef unsigned int TypeBits;

class Component : public RefCounted
{
    public:
        Component(){}
        virtual ~Component(){}

        /// \brief Serialize this component.
        virtual void serialize(sf::Packet &packet){}

        /// \brief Deserialize this component.
        virtual void deserialize(sf::Packet &packet){}

        /// \brief Get this component's type bits.
        virtual const TypeBits getTypeBits() const = 0;
};

#endif // COMPONENT_H
