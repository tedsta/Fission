#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/System.h"

class RenderSystem : public System
{
    public:
        RenderSystem(EventManager *eventManager, TypeBits renderableTypeBits = 0);
        virtual ~RenderSystem();

        // Setters

        void setBackgroundColor(sf::Color col){mBackgroundColor=col;}

        // Getters

        /// \brief Get the SFML render window
        sf::RenderWindow& getWindow(){return mWindow;}

        /// \brief Return a reference to the SFML view
        sf::View& getView(){return mView;}

    protected:
        /// \brief begin function for systems
        void begin(const float dt);

        /// \brief Process entity function for systems
        void processEntity(Entity* entity, const float dt);

        /// \brief end function for systems
        void end(const float dt);

    private:
        /// The SFML render window
        sf::RenderWindow mWindow;

        /// The SFML view
        sf::View mView;

        /// The background color
        sf::Color mBackgroundColor;
};

#endif // RENDERSYSTEM_H
