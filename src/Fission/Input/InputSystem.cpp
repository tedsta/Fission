#include <Fission/Input/InputSystem.h>

#include <SFML/Window/Event.hpp>

#include <Fission/Core/EventManager.h>
#include <Fission/Input/Defs.h>
#include <Fission/Input/IKeyboardListener.h>
#include <Fission/Input/IMouseListener.h>

namespace fsn
{
    InputSystem::InputSystem(IEventManager* eventManager, sf::Window* window) : System(eventManager), mWindow(window)
    {
        //ctor
    }

    InputSystem::~InputSystem()
    {
        //dtor
    }

    void InputSystem::begin(const float dt)
    {
        sf::Event event;
        while (mWindow->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
            {
                mWindow->close();
                break;
            }


            case sf::Event::KeyPressed:
            {
                for (auto listener : mKeyListeners)
                {
                    if (listener->onKeyPressed(event.key.code))
                        break;
                }
                break;
            }

            case sf::Event::KeyReleased:
            {
                for (auto listener : mKeyListeners)
                {
                    if (listener->onKeyReleased(event.key.code))
                        break;
                }
                break;
            }

            case sf::Event::MouseButtonPressed:
            {
                for (auto listener : mMouseListeners)
                {
                    if (listener->onMouseButtonPressed(event.mouseButton.button))
                        break;
                }
                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                for (auto listener : mMouseListeners)
                {
                    if (listener->onMouseButtonReleased(event.mouseButton.button))
                        break;
                }
                break;
            }

            case sf::Event::MouseMoved:
            {
                int moveX = event.mouseMove.x - mMousePosition.x;
                int moveY = event.mouseMove.y - mMousePosition.y;
                mMousePosition.x = event.mouseMove.x;
                mMousePosition.y = event.mouseMove.y;
                for (auto listener : mMouseListeners)
                {
                    if (listener->onMouseMoved(event.mouseMove.x, event.mouseMove.y, moveX, moveY))
                        break;
                }
                break;
            }

            default:
                break;
            }
        }
    }

    void InputSystem::processEntity(EntityRef* entity, const float dt)
    {
    }

    void InputSystem::end(const float dt)
    {
    }
}

