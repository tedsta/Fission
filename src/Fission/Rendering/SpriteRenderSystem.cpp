#include "Fission/Rendering/SpriteRenderSystem.h"

#include "Fission/Rendering/Sprite.h"

namespace fsn
{
    SpriteRenderSystem::SpriteRenderSystem(EntityManager& entityMgr, RenderManager* renderManager) :
        RenderSystem<Sprite>(entityMgr, renderManager)
    {
    }

    void SpriteRenderSystem::render(const EntityRef& entity, Sprite* sprite, sf::RenderTarget& target, sf::RenderStates& states)
    {
        if (sprite->mAnimClock.getElapsedTime().asMilliseconds() >= sprite->mFrameDelay &&
            (sprite->mLoopAnim || (sprite->mFrameDir == 1 && sprite->mCurrentFrame != sprite->mEndFrame) ||
             (sprite->mFrameDir == -1 && sprite->mCurrentFrame != sprite->mStartFrame)))
        {
            sprite->mAnimClock.restart();
            sprite->mCurrentFrame += sprite->mFrameDir;

            if (sprite->mCurrentFrame > sprite->mEndFrame && sprite->mFrameDir == 1 && sprite->mLoopAnim)
                sprite->mCurrentFrame = sprite->mStartFrame;
            else if (sprite->mCurrentFrame < sprite->mStartFrame && sprite->mFrameDir == -1 && sprite->mLoopAnim)
                sprite->mCurrentFrame = sprite->mEndFrame;
        }

        // Calculate frame position
        int frameX = (sprite->mCurrentFrame%sprite->mFramesPerRow)*sprite->mFrameDim.x;
        int frameY = (sprite->mCurrentFrame/sprite->mFramesPerRow)*sprite->mFrameDim.y;

        sprite->mSprite.setTextureRect(sf::IntRect(frameX, frameY, sprite->mFrameDim.x, sprite->mFrameDim.y));

        //*****************************************************************************************

        target.draw(sprite->mSprite, states); // Rendahhh!!!!
    }
}

