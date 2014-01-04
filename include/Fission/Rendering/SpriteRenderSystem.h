#ifndef SPRITERENDERSYSTEM_H
#define SPRITERENDERSYSTEM_H

#include "Fission/Rendering/RenderSystem.h"

namespace fsn
{
    class SpriteComponent;

    class SpriteRenderSystem : public RenderSystem<SpriteComponent>
    {
        public:
            SpriteRenderSystem(IEventManager* eventManager, RenderManager* renderManager, float lockStep);

        protected:
            void render(SpriteComponent* sprite, sf::RenderTarget& target, sf::RenderStates& states);

        private:
    };
}


#endif // SPRITERENDERSYSTEM_H
