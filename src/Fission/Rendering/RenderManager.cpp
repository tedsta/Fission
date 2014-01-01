#include "Fission/Rendering/RenderManager.h"

#include "Fission/Core/EntityRef.h"
#include "Fission/Rendering/RenderComponent.h"
#include "Fission/Rendering/RenderSystem.h"
#include "Fission/Rendering/TransformComponent.h"

RenderManager::RenderManager(int width, int height, const std::string& wndName, int layers, sf::Font* debugFont) :
    mDebugDisplay(debugFont), mLayers(layers)
{
    mWindow.create(sf::VideoMode(width, height, 32), wndName);
    mView = mWindow.getView();
}

RenderManager::~RenderManager()
{
}

void RenderManager::renderLayers()
{
    for (auto& layer : mLayers)
    {
        for (auto& renderable : layer)
        {
            sf::RenderStates states;
            states.transform = renderable.transform->getTransform();

            mRenderSystems[renderable.componentID]->render(renderable.render, mWindow, states);
        }
    }
}

void RenderManager::addRenderableToLayer(int layer, EntityRef* entity, int componentID)
{
    auto transform = entity->getComponent<TransformComponent>();
    auto render = static_cast<RenderComponent*>(entity->getComponent(componentID));
    mLayers[layer].push_back(Renderable{componentID, transform, render});
}

void RenderManager::removeRenderableFromLayer(int layer, int componentID)
{
    for (std::size_t i = 0; i < mLayers[layer].size(); i++)
    {
        if (mLayers[layer][i].componentID == componentID)
        {
            mLayers[layer].erase(mLayers[layer].begin()+i);
            return;
        }
    }
}
