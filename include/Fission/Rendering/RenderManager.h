#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <set>
#include <vector>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Fission/Core/EntityRef.h"
#include "Fission/Rendering/DebugDisplay.h"

namespace fsn
{
    class EntityRef;
    class IRenderSystem;
    class RenderComponent;
    class Transform;

    class RenderOverlay
    {
        public:
            virtual void draw(sf::RenderTarget& target) = 0;
    };

    class RenderManager
    {
        template <typename RenderComponentT> friend class RenderSystem;

        public:
            RenderManager(int width, int height, const std::string& wndName, int layers, sf::Font* debugFont);
            ~RenderManager();

            void render();

            void addOverlay(RenderOverlay* overlay){mOverlays.push_back(overlay);}

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
                EntityRef entity;
                Transform* transform;
                RenderComponent* render;
            };

            void addRenderableToLayer(int layer, const EntityRef& entity, int componentID);
            void removeRenderableFromLayer(int layer, int componentID);

            sf::RenderWindow mWindow;
            sf::View mView;

            sf::Color mBackgroundColor;

            DebugDisplay mDebugDisplay;

            std::vector<IRenderSystem*> mRenderSystems;
            std::vector<std::vector<Renderable>> mLayers;
            std::vector<RenderOverlay*> mOverlays;
    };
}

#endif // RENDERMANAGER_H
