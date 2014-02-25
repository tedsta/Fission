#ifndef FISSION_SPRITE_H
#define FISSION_SPRITE_H

#include <string>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Fission/Rendering/RenderComponent.h>

namespace fsn
{
    class Sprite : public RenderComponent
    {
        FISSION_COMPONENT

        friend class SpriteRenderSystem;

        public:
            Sprite(const std::string& texturePath = "", int frames = 1, int framesPerRow = 1);
            virtual ~Sprite();

            // Serialization stuff
            void serialize(Packet& packet);
            void deserialize(Packet& packet);

            // Renderable components gotto render...
            void render(sf::RenderTarget& target, sf::RenderStates states);
            void renderShadow(sf::RenderTarget& target, sf::RenderStates states);

            // Setters

            /// \brief Set the relative position of this sprite
            void setRelativePosition(const sf::Vector2f& pos){mRelativePosition=pos;}

            /// \brief Set the relative rotation of this sprite
            void setRelativeRotation(const float rot){mRelativeRotation=rot;}

            /// \brief Set the frame loop
            void setFrameLoop(int start, int stop);

            /// \brief Set the frame direction
            void setFrameDir(int dir){mFrameDir=dir;}

            /// \brief Set whether or not to loop
            void setLoopAnim(bool loop){mLoopAnim=loop;}

            // Getters

            /// \brief Get the SFML sprite.
            const sf::Sprite& getSprite() const {return mSprite;}
            const sf::Vector2f& getFrameDim() const {return mFrameDim;}
            sf::FloatRect getBounds() const {return sf::FloatRect(0, 0, mFrameDim.x, mFrameDim.y);}

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
}


#endif // SPRITECOMPONENT_H
