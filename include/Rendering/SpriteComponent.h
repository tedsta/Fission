#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <string>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Rendering/RenderComponent.h"

class SpriteComponent : public RenderComponent
{
    public:
        SpriteComponent(sf::Texture *texture = NULL, int frames = 1, int framesPerRow = 1);
        virtual ~SpriteComponent();

        // Serialization stuff
        void serialize(sf::Packet &packet);
        void deserialize(sf::Packet &packet);

        // Renderable components gotto render...
        void render(sf::RenderTarget& target, sf::RenderStates states);

        // Setters

        /// \brief Set the relative position of this sprite
        void setRelativePosition(const sf::Vector2f& pos){mRelativePosition=pos;}

        /// \brief Set the relative rotation of this sprite
        void setRelativeRotation(const float rot){mRelativeRotation=rot;}

        // Getters

        /// \brief Get the SFML sprite.
        const sf::Sprite& getSprite() const {return mSprite;}
        const sf::Vector2f& getFrameDim() const {return mFrameDim;}

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new SpriteComponent();}

    private:
        /// The SFML sprite to draw
        sf::Sprite mSprite;

        /// The file path to this sprite's texture
        std::string mTexturePath;

        /// The total number of frames in the animation
        int mFrames;

        /// The number of frames per row in the sprite sheet
        int mFramesPerRow;

        /// The current frame number in the animation
        int mCurrentFrame;

        /// The SFML clock
        sf::Clock mAnimClock;

        /// The time (in seconds) between frames
        float mFrameDelay;

        /// Whether or not to loop the animation
        bool mLoopAnim;

        /// The start frame of the frame loop
        int mStartFrame;

        /// The end frame of the frame loop
        int mEndFrame;

        /// The frame direction
        int mFrameDir;

        /// The sprite animation's frame dimensions
        sf::Vector2f mFrameDim;

        /// The offset of the sprite in relation to the entity
        sf::Vector2f mRelativePosition;

        /// The rotation of the sprite in relation to the entity
        float mRelativeRotation;
};

#endif // SPRITECOMPONENT_H
