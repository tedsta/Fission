#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <set>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Fission/Rendering/DebugDisplay.h"

class EntityRef;
class RenderSystem;
class RenderComponent;
class TransformComponent;

class RenderManager
{
    friend class RenderSystem;

    public:
        RenderManager(int width, int height, const std::string& wndName, int layers, sf::Font* debugFont);
        ~RenderManager();

        void renderLayers();

        // Getters

        /// \brief Get the SFML render window
        sf::RenderWindow& getWindow(){return mWindow;}

        /// \brief Return a reference to the SFML view
        sf::View& getView(){return mView;}

        /// \brief Get the debug display
        DebugDisplay& getDebugDisplay(){return mDebugDisplay;}

    private:
        struct Renderable
        {
            int componentID;
            TransformComponent* transform;
            RenderComponent* render;
        };

        void addRenderableToLayer(int layer, EntityRef* entity, int componentID);
        void removeRenderableFromLayer(int layer, int componentID);

        sf::RenderWindow mWindow;
        sf::View mView;

        sf::Color mBackgroundColor;

        DebugDisplay mDebugDisplay;

        std::vector<RenderSystem*> mRenderSystems;
        std::vector<std::vector<Renderable>> mLayers;
};

#endif // RENDERMANAGER_H
