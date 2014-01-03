#include "Fission/Rendering/RenderManager.h"

#include "Fission/Core/EntityRef.h"
#include "Fission/Rendering/RenderComponent.h"
#include "Fission/Rendering/RenderSystem.h"
#include "Fission/Rendering/TransformComponent.h"

namespace fission
{
    RenderManager::RenderManager(int width, int height, const std::string& wndName, int layers, sf::Font* debugFont) :
        mBackgroundColor(sf::Color::Black), mDebugDisplay(debugFont), mLayers(layers)
    {
        mWindow.create(sf::VideoMode(width, height, 32), wndName);
        mView = mWindow.getView();
    }

    RenderManager::~RenderManager()
    {
    }

    void RenderManager::renderLayers()
    {
        mWindow.clear(mBackgroundColor); // Clear the window
        mWindow.setView(mView);

        for (std::size_t i = 0; i < mLayers.size(); i++)
        {
            for (std::size_t j = 0; j < mLayers[i].size(); j++)
            {
                if (mLayers[i][j].render->getLayer() != static_cast<int>(i)) // If this renderable isn't in the right layer
                {
                    // Put the renderable in the right layer
                    mLayers[mLayers[i][j].render->getLayer()].push_back(mLayers[i][j]);
                    mLayers[i].erase(mLayers[i].begin()+j);
                    j--;
                    continue;
                }

                sf::RenderStates states;
                states.transform = mLayers[i][j].transform->getTransform();

                mRenderSystems[mLayers[i][j].componentID]->render(mLayers[i][j].render, mWindow, states);
            }
        }

        mWindow.setView(mWindow.getDefaultView());
        mWindow.display();
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
}

