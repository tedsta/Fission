#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Fission/Rendering/DebugDisplay.h"

class RenderManager
{
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
        sf::RenderWindow mWindow;
        sf::View mView;

        DebugDisplay mDebugDisplay;
};

#endif // RENDERMANAGER_H
