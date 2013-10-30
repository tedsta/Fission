#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include <SFML/System/Vector2.hpp>

#include "Core/Component.h"

class PlayerComponent : public Component
{
    friend class PlayerSystem;

    public:
        PlayerComponent();
        virtual ~PlayerComponent();

        // Serialization stuff
        void serialize(sf::Packet &packet);
        void deserialize(sf::Packet &packet);

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new PlayerComponent;}

    private:
        int mNetID;
        float mAnimTime;
        sf::Vector2f mCam;
        bool mStupidMode;
};

#endif // PLAYERCOMPONENT_H
