#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <Box2D/Dynamics/b2Body.h>

#include "Core/Component.h"

class RigidBodyComponent : public Component
{
    public:
        RigidBodyComponent(b2Body *body = NULL);
        virtual ~RigidBodyComponent();

        // Serialization stuff
        void serialize(sf::Packet &packet);
        void deserialize(sf::Packet &packet);

        // Getters

        /// \brief Get the Box2D body
        b2Body *getBody() const {return mBody;}

        const TypeBits getTypeBits() const {return 8;}

    private:
        b2Body *mBody;
};

#endif // RIGIDBODYCOMPONENT_H
