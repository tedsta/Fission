#ifndef RENDERABLECOMPONENT_H
#define RENDERABLECOMPONENT_H

#include <SFML/Graphics/RenderTarget.hpp>

#include <Fission/Core/Component.h>

namespace fsn
{
    class Transform;

    class RenderComponent : public Component
    {
        public:
            RenderComponent() : mLit(true), mLayer(0) {}
            virtual ~RenderComponent() {}

            virtual void serialize(Packet& packet)
            {
                packet << mLit;
                packet << mLayer;
            }

            virtual void deserialize(Packet& packet)
            {
                packet >> mLit;
                packet >> mLayer;
            }

            virtual void render(sf::RenderTarget& target, sf::RenderStates states){}
            virtual void renderShadow(sf::RenderTarget& target, sf::RenderStates states){}
            virtual void renderLit(sf::RenderTarget& target, sf::RenderStates states){}

            // Setters

            void setLit(bool lit){mLit=lit;}
            void setLayer(int layer){mLayer=layer;}

            // Getters

            bool getLit() const {return mLit;}
            int getLayer() const {return mLayer;}
            virtual sf::FloatRect getBounds() const {return sf::FloatRect(0, 0, 0, 0);}

        private:
            bool mLit;
            int mLayer;
    };
}


#endif // RENDERABLECOMPONENT_H
