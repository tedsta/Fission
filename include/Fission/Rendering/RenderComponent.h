#ifndef RENDERABLECOMPONENT_H
#define RENDERABLECOMPONENT_H

#include <SFML/Graphics/RenderTarget.hpp>

#include <Fission/Core/Component.h>

class TransformComponent;

class RenderComponent : public Component
{
    public:
        RenderComponent() {}
        virtual ~RenderComponent() {}

        virtual void render(sf::RenderTarget& target, sf::RenderStates states) = 0;

    private:
};

#endif // RENDERABLECOMPONENT_H
