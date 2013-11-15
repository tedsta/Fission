#include <Fission/Input/InputSystem.h>

#include <SFML/Window/Event.hpp>

#include <Fission/Core/EventManager.h>
#include <Fission/Input/Defs.h>
#include <Fission/Input/Events.h>

InputSystem::InputSystem(EventManager *eventManager, sf::Window *window) : System(eventManager, 0), mWindow(window)
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
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            getEventManager()->fireEvent(KeyEvent(event.key.code, BtnState::PRESSED));
            break;
        case sf::Event::KeyReleased:
            getEventManager()->fireEvent(KeyEvent(event.key.code, BtnState::RELEASED));
            break;
        case sf::Event::MouseButtonPressed:
            getEventManager()->fireEvent(MouseBtnEvent(event.mouseButton.button, BtnState::PRESSED));
            break;
        case sf::Event::MouseButtonReleased:
            getEventManager()->fireEvent(MouseBtnEvent(event.mouseButton.button, BtnState::RELEASED));
            break;
        case sf::Event::MouseMoved:
            getEventManager()->fireEvent(MouseMoveEvent(event.mouseMove.x, event.mouseMove.y));
            break;
        default:
            break;
        }
    }
}

void InputSystem::processEntity(Entity* entity, const float dt)
{
}

void InputSystem::end(const float dt)
{
}
