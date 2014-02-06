#include "Fission/Rendering/RenderManager.h"

#include "Fission/Core/EntityRef.h"
#include "Fission/Rendering/RenderComponent.h"
#include "Fission/Rendering/RenderSystem.h"
#include "Fission/Rendering/Transform.h"

namespace fsn
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

    void RenderManager::render()
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

                if (mLayers[i][j].transform != nullptr)
                    states.transform = mLayers[i][j].transform->getGlobalTransform();

                mRenderSystems[mLayers[i][j].componentID]->render(mLayers[i][j].entity, mLayers[i][j].render, mWindow, states);
            }
        }

        mWindow.setView(mWindow.getDefaultView());
        for (auto overlay : mOverlays)
            overlay->draw(mWindow);
        mDebugDisplay.render(mWindow);
        mWindow.display();
    }

    void RenderManager::addRenderableToLayer(int layer, const EntityRef& entity, int componentID)
    {
        auto transform = entity.getComponent<Transform>();
        auto render = static_cast<RenderComponent*>(entity.getComponent(componentID));
        mLayers[layer].push_back(Renderable{componentID, entity, transform, render});
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

