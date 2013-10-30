#include "Physics/RigidBodyComponent.h"

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

RigidBodyComponent::RigidBodyComponent(b2Body *body) : mBody(body)
{
    // Set this as the user data
    mBody->SetUserData(this);
}

RigidBodyComponent::~RigidBodyComponent()
{
    mBody->GetWorld()->DestroyBody(mBody);
}

void RigidBodyComponent::serialize(sf::Packet &packet)
{
    Component::serialize(packet);

    packet << int(mBody->GetType());

    packet << mBody->GetPosition().x;
    packet << mBody->GetPosition().y;
    packet << mBody->GetAngle();

    packet << sf::Int8(mBody->IsFixedRotation());
    packet << sf::Int8(mBody->IsSleepingAllowed());

    //get the fixture count
    sf::Int32 fixtureCount = 0;
    for (b2Fixture* f = mBody->GetFixtureList(); f; f = f->GetNext())
        fixtureCount++;
    packet << fixtureCount;

    for (b2Fixture* f = mBody->GetFixtureList(); f; f = f->GetNext())
    {
        packet << f->GetDensity();
        packet << f->GetFriction();
        packet << sf::Int8(f->IsSensor());
        packet << sf::Int32(f->GetFilterData().categoryBits);
        packet << sf::Int32(f->GetFilterData().maskBits);
        packet << sf::Int32(f->GetFilterData().groupIndex);
        b2Shape::Type shapeType = f->GetType();
        packet << int(shapeType);
        if (shapeType == b2Shape::e_circle)
        {
            b2CircleShape* circleShape = (b2CircleShape*)f->GetShape();
            packet << circleShape->m_radius;
        }
        else if (shapeType == b2Shape::e_polygon)
        {
            b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
            packet << polygonShape->GetVertexCount();
            for (int v = 0; v < polygonShape->GetVertexCount(); v++)
            {
                packet << polygonShape->GetVertex(v).x;
                packet << polygonShape->GetVertex(v).y;
            }
        }
    }
}

void RigidBodyComponent::deserialize(sf::Packet &packet)
{
    Component::deserialize(packet);

    b2BodyDef bodyDef;

    int bodyType;
    packet >> bodyType;
    bodyDef.type = (b2BodyType)bodyType;

    packet >> bodyDef.position.x;
    packet >> bodyDef.position.y;
    packet >> bodyDef.angle;

    sf::Int8 fixedRotation;
    sf::Int8 allowSleep;
    packet >> fixedRotation;
    packet >> allowSleep;
    bodyDef.fixedRotation = fixedRotation;
    bodyDef.allowSleep = allowSleep;

    //mBody = getGame()->getPhysicsManager()->getWorld()->CreateBody(&bodyDef);
    mBody->SetUserData(this); //set the user data to this object

    //get the fixture count
    int fixtureCount = 0;
    packet >> fixtureCount;

    for (int f = 0; f < fixtureCount; f++)
    {
        b2FixtureDef fixtureDef;
        b2Shape *shape = NULL;
        sf::Int8 isSensor;
        packet >> fixtureDef.density;
        packet >> fixtureDef.friction;
        packet >> isSensor;
        fixtureDef.isSensor = isSensor;

        sf::Int32 categoryBits, maskBits, groupIndex;
        packet >> categoryBits;
        packet >> maskBits;
        packet >> groupIndex;

        fixtureDef.filter.categoryBits = categoryBits;
        fixtureDef.filter.maskBits = maskBits;
        fixtureDef.filter.groupIndex = groupIndex;

        sf::Int32 shapeType;
        packet >> shapeType;
        if (shapeType == b2Shape::e_circle)
        {
            b2CircleShape* circleShape = new b2CircleShape;
            shape = circleShape;
            packet >> circleShape->m_radius;
        }
        else if (shapeType == b2Shape::e_polygon)
        {
            b2PolygonShape *polygonShape = new b2PolygonShape;
            shape = polygonShape;
            sf::Int32 vertexCount;
            packet >> vertexCount;
            b2Vec2 *vecs = new b2Vec2[vertexCount];
            for (int v = 0; v < vertexCount; v++)
            {
                packet >> vecs[v].x;
                packet >> vecs[v].y;
            }
            polygonShape->Set(vecs, vertexCount);
            delete[] vecs;
        }

        if (shape)
        {
            fixtureDef.shape = shape;
            mBody->CreateFixture(&fixtureDef);
        }
        delete shape;
    }
}
