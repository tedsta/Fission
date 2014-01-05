#ifndef SPRITERENDERSYSTEM_H
#define SPRITERENDERSYSTEM_H

#include "Fission/Rendering/RenderSystem.h"

namespace fsn
{
    class Sprite;

    class SpriteRenderSystem : public RenderSystem<Sprite>
    {
        public:
            SpriteRenderSystem(IEventManager* eventManager, RenderManager* renderManager, float lockStep);

        protected:
            void render(Sprite* sprite, sf::RenderTarget& target, sf::RenderStates& states);

        private:
    };
}


#endif // SPRITERENDERSYSTEM_H
