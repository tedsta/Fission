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

        protected:
            void begin(const float dt);

            void processEntity(EntityRef* entity, const float dt);

            void end(const float dt);

        private:
            sf::Window *mWindow;
            std::vector<IKeyboardListener*> mKeyListeners;
            std::vector<IMouseListener*> mMouseListeners;
    };
}


#endif // INPUTSYSTEM_H
