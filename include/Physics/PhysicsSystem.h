#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Body.h>
#include <SFML/System/Vector2.hpp>

#include "Core/System.h"

class PhysicsSystem : public System
{
    public:
        PhysicsSystem(EventManager *eventManager);
        virtual ~PhysicsSystem();

        /// \brief Create a box body.
        /// \param dim The dimensions of the box to create
        b2Body *createBox(const sf::Vector2f& dim, float density);

    protected:
        void begin(const float dt);
        void processEntity(Entity *entity, const float dt);

    private:
        b2World *mWorld;
};

#endif // PHYSICSSYSTEM_H
