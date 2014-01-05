#ifndef FISSION_TRANSFORM_H
#define FISSION_TRANSFORM_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Network/Packet.hpp>

#include <Fission/Core/Component.h>

namespace fsn
{
    class Transform : public Component, public sf::Transformable
    {
        FISSION_COMPONENT

        public:
            Transform(sf::Vector2f pos = sf::Vector2f(0, 0), float rot = 0, sf::Vector2f scale = sf::Vector2f(1, 1));
            virtual ~Transform();

            void serialize(sf::Packet& packet);
            void deserialize(sf::Packet& packet);

        private:
    };
}


#endif // TRANSFORMCOMPONENT_H
