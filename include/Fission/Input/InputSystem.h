#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SFML/Window/Window.hpp>

#include <Fission/Core/Systems/System.h>

namespace fsn
{
    class IKeyboardListener;
    class IMouseListener;

    class InputSystem : public System
    {
        public:
            InputSystem(sf::Window* window);
            virtual ~InputSystem();

            void addKeyboardListener(IKeyboardListener* listener){mKeyListeners.push_back(listener);}
            void addMouseListener(IMouseListener* listener){mMouseListeners.push_back(listener);}

            void update(const float dt);

        private:
            sf::Window *mWindow;
            std::vector<IKeyboardListener*> mKeyListeners;
            std::vector<IMouseListener*> mMouseListeners;

            sf::Vector2i mMousePosition;
    };
}


#endif // INPUTSYSTEM_H
