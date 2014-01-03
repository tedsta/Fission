#include "Fission/Rendering/SpriteRenderSystem.h"

#include "Fission/Rendering/SpriteComponent.h"

namespace fission
{
    SpriteRenderSystem::SpriteRenderSystem(IEventManager* eventManager, RenderManager* renderManager, float lockStep) :
        RenderSystem(eventManager, renderManager, lockStep)
    {
        initialize<SpriteComponent>();
    }

    void SpriteRenderSystem::render(RenderComponent* rnd, sf::RenderTarget& target, sf::RenderStates& states)
    {
        auto sprite = static_cast<SpriteComponent*>(rnd);

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

