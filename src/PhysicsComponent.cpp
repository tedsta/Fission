#include "PhysicsComponent.h"

#include "GridComponent.h"

TypeBits PhysicsComponent::Type;

PhysicsComponent::PhysicsComponent() : mGravityDir(DOWN), mGrid(NULL)
{
    //ctor
}

PhysicsComponent::~PhysicsComponent()
{
    //dtor
}

void PhysicsComponent::serialize(sf::Packet &packet)
{
}

void PhysicsComponent::deserialize(sf::Packet &packet)
{
}
