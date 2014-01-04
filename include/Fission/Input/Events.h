#ifndef INPUT_EVENTS_H_INCLUDED
#define INPUT_EVENTS_H_INCLUDED

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Fission/Core/EntityEvents.h>
#include <Fission/Input/Defs.h>

namespace fsn
{
    enum
    {
        EVT_KEY = 100,
        EVT_MOUSE_BTN,
        EVT_MOUSE_MOVE
    };

    // KeyEvent
    struct KeyEvent : public IEventData
    {
        KeyEvent(sf::Keyboard::Key key, ButtonState state) : IEventData(EVT_KEY), mKey(key), mState(state) {}
        static EventID Type;

        sf::Keyboard::Key mKey;
        ButtonState mState;
    };

    // MouseBtnEvent
    struct MouseBtnEvent : public IEventData
    {
        MouseBtnEvent(sf::Mouse::Button btn, ButtonState state) : IEventData(EVT_MOUSE_BTN), mBtn(btn), mState(state) {}

        sf::Mouse::Button mBtn;
        ButtonState mState;
    };

    // MouseMoveEvent
    struct MouseMoveEvent : public IEventData
    {
        MouseMoveEvent(int x, int y) : IEventData(EVT_MOUSE_MOVE), mX(x), mY(y) {}

        int mX, mY;
    };
}


#endif // INPUT_EVENTS_H_INCLUDED
