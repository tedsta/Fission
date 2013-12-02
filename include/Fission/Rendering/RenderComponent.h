#ifndef RENDERABLECOMPONENT_H
#define RENDERABLECOMPONENT_H

#include <SFML/Graphics/RenderTarget.hpp>

#include <Fission/Core/Component.h>

class TransformComponent;

class RenderComponent : public Component
{
    public:
        RenderComponent() : lit(true) {}
        virtual ~RenderComponent() {}

        virtual void render(sf::RenderTarget& target, sf::RenderStates states){}
        virtual void renderShadow(sf::RenderTarget& target, sf::RenderStates states){}

        void setLit(bool _lit){lit=_lit;}

        bool getLit(){return lit;}

    private:
        bool lit;
};

#endif // RENDERABLECOMPONENT_H
