#include "Fission/Rendering/RenderManager.h"

RenderManager::RenderManager(int width, int height, const std::string& wndName, int layers, sf::Font* debugFont) :
    mDebugDisplay(debugFont)
{
    mWindow.create(sf::VideoMode(width, height, 32), wndName);
    mView = mWindow.getView();
}

RenderManager::~RenderManager()
{
}

void RenderManager::renderLayers()
{
}
