#include <Fission/Rendering/Sprite.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Fission/Core/ResourceManager.h>
#include <Fission/Rendering/Transform.h>

namespace fsn
{
    Sprite::Sprite(const std::string& texturePath, int frames, int framesPerRow) :
        mTexturePath(texturePath), mFrames(frames), mFramesPerRow(framesPerRow), mCurrentFrame(0), mFrameDelay(100), mLoopAnim(true),
        mStartFrame(0), mEndFrame(mFrames-1), mFrameDir(1), mRelativeRotation(0)
    {
        if (mTexturePath.size() == 0)
            return;

        mSprite = sf::Sprite(*ResourceManager::get()->getTexture(mTexturePath));

        // Calculate frame dimensions
        mFrameDim = sf::Vector2f(mSprite.getTexture()->getSize().x/mFramesPerRow, mSprite.getTexture()->getSize().y/(mFrames/mFramesPerRow));
    }

    Sprite::~Sprite()
    {
        //dtor
    }

    void Sprite::serialize(Packet& packet)
    {
        RenderComponent::serialize(packet);

        packet << mTexturePath;
        packet << mCurrentFrame << mFrameDir << mFrameDelay << sf::Int8(mLoopAnim);
        packet << mFrames << mFramesPerRow << mStartFrame << mEndFrame;
        packet << mRelativePosition.x << mRelativePosition.y << mRelativeRotation;
    }

    void Sprite::deserialize(Packet& packet)
    {
        RenderComponent::deserialize(packet);

        packet >> mTexturePath;
        mSprite = sf::Sprite(*ResourceManager::get()->getTexture(mTexturePath));

        //base animation stuff
        packet >> mCurrentFrame >> mFrameDir >> mFrameDelay;

        sf::Int8 loop;
        packet >> loop;
        mLoopAnim = loop;

        packet >> mFrames >> mFramesPerRow >> mStartFrame >> mEndFrame;

        // Calculate frame dimensions
        mFrameDim = sf::Vector2f(mSprite.getTexture()->getSize().x/mFramesPerRow, mSprite.getTexture()->getSize().y/(mFrames/mFramesPerRow));

        packet >> mRelativePosition.x >> mRelativePosition.y >> mRelativeRotation;
    }

    void Sprite::render(sf::RenderTarget& target, sf::RenderStates states)
    {
        if (mAnimClock.getElapsedTime().asMilliseconds() >= mFrameDelay &&
            (mLoopAnim || (mFrameDir == 1 && mCurrentFrame != mEndFrame) || (mFrameDir == -1 && mCurrentFrame != mStartFrame)))
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

    void Sprite::renderShadow(sf::RenderTarget& target, sf::RenderStates states)
    {
        mSprite.setColor(sf::Color::Black);
        target.draw(mSprite, states); // Rendahhh!!!!
    }

    void Sprite::setFrameLoop(int start, int stop)
    {
        if (start <= stop)
        {
            mStartFrame = start;
            mEndFrame = stop;
            mCurrentFrame = start;
            mFrameDir = 1;
        }
        else
        {
            mStartFrame = stop;
            mEndFrame = start;
            mCurrentFrame = start;
            mFrameDir = -1;
        }
    }
}

