#include <Fission/Rendering/RenderSystem.h>

#include <Fission/Rendering/DebugDisplay.h>
#include <Fission/Rendering/TransformComponent.h>
#include <Fission/Rendering/RenderComponent.h>
#include <Fission/Rendering/SpriteComponent.h>

#include <iostream>

RenderSystem::RenderSystem(EventManager *eventManager, sf::Font* debugFont, TypeBits renderableTypeBits) :
    System(eventManager, TransformComponent::Type, SpriteComponent::Type|renderableTypeBits)
{
    mWindow.create(sf::VideoMode(800,600,32), "Fission");
    mView = mWindow.getView();
    mDebugDisplay = new DebugDisplay(debugFont);
}

RenderSystem::~RenderSystem()
{
    //mWindow.close();
    delete mDebugDisplay;
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
    mWindow.setView(mWindow.getDefaultView());
    mDebugDisplay->render(mWindow);
    mWindow.display(); // Display to the window
}
