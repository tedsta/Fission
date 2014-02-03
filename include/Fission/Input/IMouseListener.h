#ifndef IMOUSELISTENER_H
#define IMOUSELISTENER_H

#include <SFML/Window/Mouse.hpp>

namespace fsn
{
    class IMouseListener
    {
        public:
            virtual bool onMouseButtonPressed(sf::Mouse::Button button) = 0;
            virtual bool onMouseButtonReleased(sf::Mouse::Button button) = 0;
            virtual bool onMouseMoved(int posX, int posY, int moveX, int moveY) = 0;
    };
}

#endif // IMOUSELISTENER_H
