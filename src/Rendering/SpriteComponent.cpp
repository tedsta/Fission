#include "Rendering/SpriteComponent.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Rendering/TransformComponent.h"

TypeBits SpriteComponent::Type;

SpriteComponent::SpriteComponent(sf::Texture *texture, int frames, int framesPerRow) :
    mSprite(*texture), mFrames(frames), mFramesPerRow(framesPerRow), mCurrentFrame(0), mFrameDelay(100), mLoopAnim(true),
    mStartFrame(0), mEndFrame(mFrames-1), mFrameDir(1), mRelativeRotation(0)
{
    // Calculate frame dimensions
    mFrameDim = sf::Vector2f(texture->getSize().x/mFramesPerRow, texture->getSize().y/(mFrames/mFramesPerRow));
}

SpriteComponent::~SpriteComponent()
{
    //dtor
}

void SpriteComponent::serialize(sf::Packet &packet)
{
    Component::serialize(packet);

    packet << mTexturePath;
    packet << mCurrentFrame << mFrameDir << mFrameDelay << sf::Int8(mLoopAnim) << mFrames << mFramesPerRow;
    packet << mRelativePosition.x << mRelativePosition.y << mRelativeRotation;
}

void SpriteComponent::deserialize(sf::Packet &packet)
{
    Component::deserialize(packet);

    packet >> mTexturePath;
    //sf::Texture *texture = getGame()->getResourceManager()->getTexture(mTexturePath);
    //setTexture(texture);

    //base animation stuff
    packet >> mCurrentFrame >> mFrameDir >> mFrameDelay;

    sf::Int8 loop;
    packet >> loop;
    mLoopAnim = loop;

    packet >> mFrames >> mFramesPerRow;

    //mFrameDim = sf::Vector2i(texture->getSize().x/mFramesPerRow, texture->getSize().y/(mFrames/mFramesPerRow));
    mStartFrame = 0;
    mEndFrame = mFrames-1;

    packet >> mRelativePosition.x >> mRelativePosition.y >> mRelativeRotation;
}

void SpriteComponent::render(sf::RenderTarget& target, sf::RenderStates states)
{
    if (mAnimClock.getElapsedTime().asMilliseconds() >= mFrameDelay && (mLoopAnim || mCurrentFrame != mEndFrame))
    {
        mAnimClock.restart();
        mCurrentFrame += mFrameDir;

        if (mCurrentFrame > mEndFrame && mFrameDir == 1 && mLoopAnim)
            mCurrentFrame = mStartFrame;
        else if (mCurrentFrame < mStartFrame && mFrameDir == -1 && mLoopAnim)
            mCurrentFrame = mEndFrame;
    }

    // Calculate frame position
    int frameX = (mCurrentFrame%mFramesPerRow)*mFrameDim.x;
    int frameY = (mCurrentFrame/mFramesPerRow)*mFrameDim.y;

    mSprite.setTextureRect(sf::IntRect(frameX, frameY, mFrameDim.x, mFrameDim.y));

    // Stop here
    //*****************************************************************************************

    target.draw(mSprite, states); // Rendahhh!!!!
}
