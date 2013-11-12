#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <Fission/Core/Component.h>

class TransformComponent : public Component, public sf::Transformable
{
    public:
        TransformComponent(sf::Vector2f pos = sf::Vector2f(0, 0), float rot = 0, sf::Vector2f scale = sf::Vector2f(1, 1));
        virtual ~TransformComponent();

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new TransformComponent();}

    private:
};

#endif // TRANSFORMCOMPONENT_H
