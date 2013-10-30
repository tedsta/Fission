#include "TestSystem.h"

#include <iostream>

#include "Rendering/TransformComponent.h"
#include "Network/IntentComponent.h"

TestSystem::TestSystem(EventManager *eventManager) : System(eventManager, TypeBits(1|4|8))
{
    //ctor
}

TestSystem::~TestSystem()
{
    //dtor
}

void TestSystem::begin(const float dt)
{
}

void TestSystem::processEntity(Entity *entity, const float dt)
{
    auto trans = static_cast<TransformComponent*>(entity->getComponent(TransformComponent::Type));
    auto intent = static_cast<IntentComponent*>(entity->getComponent(IntentComponent::Type));

    if (intent->isIntentActive("up"))
        trans->move(sf::Vector2f(0, -100.f*dt));
    if (intent->isIntentActive("down"))
        trans->move(sf::Vector2f(0, 100.f*dt));
    if (intent->isIntentActive("left"))
        trans->move(sf::Vector2f(-100.f*dt, 0));
    if (intent->isIntentActive("right"))
        trans->move(sf::Vector2f(100.f*dt, 0));
}

void TestSystem::end(const float dt)
{
}
