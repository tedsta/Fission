#ifndef INPUT_EVENTS_H_INCLUDED
#define INPUT_EVENTS_H_INCLUDED

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <Fission/Core/EntityEvents.h>
#include <Fission/Input/Defs.h>

enum
{
    EVT_KEY = 100,
    EVT_MOUSE_BTN,
    EVT_MOUSE_MOVE
};

// KeyEvent
struct KeyEvent : public IEventData
{
    KeyEvent(sf::Keyboard::Key key, int state) : IEventData(EVT_KEY), mKey(key), mState(state) {}
    static EventType Type;

	sf::Keyboard::Key mKey;
	int mState;
};

// MouseBtnEvent
struct MouseBtnEvent : public IEventData
{
    MouseBtnEvent(sf::Mouse::Button btn, int state) : IEventData(EVT_MOUSE_BTN), mBtn(btn), mState(state) {}

	sf::Mouse::Button mBtn;
	int mState;
};

// MouseMoveEvent
struct MouseMoveEvent : public IEventData
{
    MouseMoveEvent(int x, int y) : IEventData(EVT_MOUSE_MOVE), mX(x), mY(y) {}

	int mX, mY;
};

#endif // INPUT_EVENTS_H_INCLUDED
