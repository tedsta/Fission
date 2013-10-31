#include "Rendering/RenderSystem.h"

#include "Rendering/TransformComponent.h"
#include "Rendering/RenderComponent.h"
#include "Rendering/SpriteComponent.h"

#include <iostream>

RenderSystem::RenderSystem(EventManager *eventManager, TypeBits renderableTypeBits) :
    System(eventManager, TransformComponent::Type, SpriteComponent::Type|renderableTypeBits)
{
    mWindow.create(sf::VideoMode(800,600,32), "Fission");
    mView = mWindow.getView();
}

RenderSystem::~RenderSystem()
{
    //mWindow.close();
}

void RenderSystem::begin(const float dt)
{
    mWindow.clear(mBackgroundColor); // Clear the window
    mWindow.setView(mView);
}

void RenderSystem::processEntity(Entity* entity, const float dt)
{
    auto transform = static_cast<TransformComponent*>(entity->getComponent(TransformComponent::Type));
    auto renderCmpnts = entity->getComponents(getOptBits());

    sf::RenderStates states = sf::RenderStates::Default;
    states.transform.combine(transform->getTransform());

    for (auto cmpnt : renderCmpnts)
    {
        static_cast<RenderComponent*>(cmpnt)->render(mWindow, states);
    }
}

void RenderSystem::end(const float dt)
{
    mWindow.display(); // Display to the window
}
