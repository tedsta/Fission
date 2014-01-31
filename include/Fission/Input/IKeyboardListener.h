#ifndef IKEYBOARDLISTENER_H
#define IKEYBOARDLISTENER_H

#include <SFML/Window/Keyboard.hpp>

namespace fsn
{
    class IKeyboardListener
    {
        public:
            virtual bool onKeyPressed(sf::Keyboard::Key key) = 0;
            virtual bool onKeyReleased(sf::Keyboard::Key key) = 0;
    };
}

#endif // IKEYBOARDLISTENER_H
