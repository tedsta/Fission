#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <SFML/System/Vector2.hpp>

#include "Core/Component.h"

class Entity;

class PhysicsComponent : public Component
{
    friend class PhysicsSystem;

    public:
        PhysicsComponent();
        virtual ~PhysicsComponent();

        // Serialization stuff
        void serialize(sf::Packet &packet);
        void deserialize(sf::Packet &packet);

        // Setters
        void setVelocity(sf::Vector2f vel){mVelocity=vel;}
        void setVelocityX(float x){mVelocity.x=x;}
        void setVelocityY(float y){mVelocity.y=y;}

        // Getters
        sf::Vector2f getVelocity(){return mVelocity;}
        Entity* getGrid(){return mGrid;}
        int getGravityDir(){return mGravityDir;}
        bool getDirCollision(int dir){return mDirCollisions[dir];}

        static TypeBits Type;
        const TypeBits getTypeBits() const {return Type;}
        static Component* factory() {return new PhysicsComponent;}

    private:
        sf::Vector2f mVelocity;
        Entity* mGrid; // Cached grid
        int mGravityDir; // Direction of gravity
        bool mDirCollisions[4];
};

#endif // PHYSICSCOMPONENT_H
