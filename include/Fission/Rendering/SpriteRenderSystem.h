#ifndef SPRITERENDERSYSTEM_H
#define SPRITERENDERSYSTEM_H

#include "Fission/Rendering/RenderSystem.h"

class SpriteComponent;

class SpriteRenderSystem : public RenderSystem
{
    public:
        SpriteRenderSystem(IEventManager* eventManager, RenderManager* renderManager, float lockStep);

    protected:
        void render(RenderComponent* sprite, sf::RenderTarget& target, sf::RenderStates& states);

    private:
};

#endif // SPRITERENDERSYSTEM_H
