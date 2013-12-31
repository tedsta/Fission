#include <Fission/Rendering/RenderSystem.h>

#include <Fission/Rendering/DebugDisplay.h>
#include <Fission/Rendering/TransformComponent.h>
#include <Fission/Rendering/RenderComponent.h>
#include <Fission/Rendering/SpriteComponent.h>

#include <iostream>

RenderSystem::RenderSystem(IEventManager* eventManager, float lockStep) :
    System(eventManager, lockStep)
{
    //mWindow.create(sf::VideoMode(800,600,32), "Fission");
    //mView = mWindow.getView();
}

RenderSystem::~RenderSystem()
{
    //mWindow.close();
}

void RenderSystem::begin(const float dt)
{
    /*mWindow.clear(mBackgroundColor); // Clear the window
    mWindow.setView(mView);

    // Calculate the number of layers
    int layers = 0;
    for (auto entity : getActiveEntities())
    {
        auto renderCmpnts = entity->getComponents(getOptBits());

        for (auto cmpnt : renderCmpnts)
        {
            RenderComponent* rCmpnt = static_cast<RenderComponent*>(cmpnt);
            if (rCmpnt->getLayer() > layers)
            {
                layers = rCmpnt->getLayer();
            }
        }
    }

    for (int layer = 0; layer <= layers; layer++)
    {
        for (auto entity : getActiveEntities())
        {
            auto transform = static_cast<TransformComponent*>(entity->getComponent(TransformComponent::Type));
            auto renderCmpnts = entity->getComponents(getOptBits());

            sf::RenderStates states = sf::RenderStates::Default;
            states.transform.combine(transform->getTransform());

            for (auto cmpnt : renderCmpnts)
            {
                RenderComponent* rCmpnt = static_cast<RenderComponent*>(cmpnt);
                if (rCmpnt->getLayer() == layer)
                {
                    rCmpnt->render(mWindow, states);
                }
            }
        }
    }*/
}

void RenderSystem::processEntity(EntityRef* entity, const float dt)
{
}

void RenderSystem::end(const float dt)
{
    /*mWindow.setView(mWindow.getDefaultView());
    for (auto sprite : mAdditionalSprites)
        mWindow.draw(*sprite);
    mDebugDisplay->render(mWindow);
    mWindow.display(); // Display to the window

    */
}
