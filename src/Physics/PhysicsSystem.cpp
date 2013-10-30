#include "Physics/PhysicsSystem.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Fixture.h>

#include "Core/Math.h"
#include "Core/TransformComponent.h"
#include "Physics/RigidBodyComponent.h"

PhysicsSystem::PhysicsSystem(EventManager *eventManager) : System(eventManager, 1|8)
{
    mWorld = new b2World(b2Vec2(0.f, -9.8f));
}

PhysicsSystem::~PhysicsSystem()
{
    //dtor
}

void PhysicsSystem::begin(const float dt)
{
    for (Entity *entity : getActiveEntities())
    {
        TransformComponent *transform = entity->getComponent<TransformComponent>();
        b2Body *body = entity->getComponent<RigidBodyComponent>()->getBody();

        body->SetTransform(b2Vec2(transform->getPosition().x, transform->getPosition().y), degToRad(transform->getRotation()));
    }

    mWorld->Step(dt, 8, 3); // Step the physics simulation
}

void PhysicsSystem::processEntity(Entity *entity, const float dt)
{
    TransformComponent *transform = entity->getComponent<TransformComponent>();
    b2Body *body = entity->getComponent<RigidBodyComponent>()->getBody();

    if (body)
    {
        transform->setPosition(sf::Vector2f(body->GetPosition().x, body->GetPosition().y));
        transform->setRotation(radToDeg(body->GetAngle()));
    }
}

b2Body *PhysicsSystem::createBox(const sf::Vector2f& dim, float density)
{
    b2BodyDef def;
    if (density <= 0)
        def.type = b2_staticBody;
    else
        def.type = b2_dynamicBody;

    b2Body *body = mWorld->CreateBody(&def);

    b2PolygonShape *shape = new b2PolygonShape;
    shape->SetAsBox((dim.x/2.f)/PTU, (dim.y/2.f)/PTU);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = shape;
    fixtureDef.friction = 0.3f;
    fixtureDef.density = density;

    body->CreateFixture(&fixtureDef);
    delete shape; //delete the shape - we're done with it

    return body;
}
