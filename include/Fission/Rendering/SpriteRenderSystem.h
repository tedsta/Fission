#ifndef SPRITERENDERSYSTEM_H
#define SPRITERENDERSYSTEM_H

#include "Fission/Rendering/RenderSystem.h"

namespace fsn
{
    class Sprite;

    class SpriteRenderSystem : public RenderSystem<Sprite>
    {
        public:
            SpriteRenderSystem(EntityManager& entityMgr, RenderManager* renderManager);

        protected:
            void render(const EntityRef& entity, Sprite* sprite, sf::RenderTarget& target, sf::RenderStates& states);

        private:
    };
}


#endif // SPRITERENDERSYSTEM_H
