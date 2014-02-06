#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SFML/Window/Window.hpp>

#include <Fission/Core/System.h>

namespace fsn
{
    class IKeyboardListener;
    class IMouseListener;

    class InputSystem : public System
    {
        public:
            InputSystem(IEventManager* eventManager, sf::Window* window);
            virtual ~InputSystem();

            void addKeyboardListener(IKeyboardListener* listener){mKeyListeners.push_back(listener);}
            void addMouseListener(IMouseListener* listener){mMouseListeners.push_back(listener);}

        protected:
            void begin(const float dt);

            void processEntity(const EntityRef& entity, const float dt);

            void end(const float dt);

        private:
            sf::Window *mWindow;
            std::vector<IKeyboardListener*> mKeyListeners;
            std::vector<IMouseListener*> mMouseListeners;

            sf::Vector2i mMousePosition;
    };
}


#endif // INPUTSYSTEM_H
