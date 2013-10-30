#include "Network/IntentSystem.h"

#include "Core/EventManager.h"
#include "Input/Defs.h"
#include "Input/Events.h"
#include "Network/Connection.h"
#include "Network/IntentComponent.h"
#include "Network/NetworkEvents.h"

#include <iostream>

IntentSystem::IntentSystem(EventManager *eventManager, Connection *conn) : System(eventManager, IntentComponent::Type), mConn(conn)
{
    mHndID = mConn->registerHandlerAuto(this);
	if (mConn->getType() == NetType::CLIENT || mConn->getType() == NetType::NONE)
	{
		getEventManager()->addListener(this, EVT_KEY);
		getEventManager()->addListener(this, EVT_MOUSE_BTN);
		getEventManager()->addListener(this, EVT_MOUSE_MOVE);
	}
}

IntentSystem::~IntentSystem()
{
    //dtor
}

void IntentSystem::begin(const float dt)
{
}

void IntentSystem::processEntity(Entity* entity, const float dt)
{
    auto intent = static_cast<IntentComponent*>(entity->getComponent(IntentComponent::Type));

	for (auto& it : intent->mIntents)
		it.second = false;

	for (int k = 0; k < sf::Keyboard::KeyCount; k++)
    {
		if (intent->mKeyStates[k] == BtnState::PRESSED)
			intent->mKeyStates[k] = BtnState::DOWN;
		else if (intent->mKeyStates[k] == BtnState::RELEASED)
			intent->mKeyStates[k] = BtnState::UP;

		auto in = IntentComponent::Action(IntentComponent::KEYBOARD, k, intent->mKeyStates[k]);

		if (intent->mInputMap.find(in) != intent->mInputMap.end())
		{
			intent->mIntents[intent->mInputMap[in]] = true;
		}
	}

	for (int m = 0; m < sf::Mouse::ButtonCount; m++)
    {
		if (intent->mMouseStates[m] == BtnState::PRESSED)
			intent->mMouseStates[m] = BtnState::DOWN;
		else if (intent->mMouseStates[m] == BtnState::RELEASED)
			intent->mMouseStates[m] = BtnState::UP;

		auto in = IntentComponent::Action(IntentComponent::MOUSE_BTN, m, intent->mMouseStates[m]);

		if (intent->mInputMap.find(in) != intent->mInputMap.end())
			intent->mIntents[intent->mInputMap[in]] = true;
	}

	// ###
	// Event handling
	if ((mConn->getType() == NetType::CLIENT && mConn->getNetID() == intent->mNetID) || mConn->getType() == NetType::NONE)
    {
		for (auto e : mEvents)
		{
			switch (e->getType())
			{
			case EVT_KEY:
            {
                auto ke = static_cast<KeyEvent*>(e);

				// Send it across the network
				if (ke->mState == BtnState::PRESSED || ke->mState == BtnState::RELEASED)
                {
					sf::Packet packet;
					packet << intent->mNetID << int(EVT_KEY) << ke->mKey << ke->mState;
					mConn->send(packet, mHndID, 0, 0, false);

					intent->mKeyStates[int(ke->mKey)] = ke->mState;

                    auto in = IntentComponent::Action(IntentComponent::KEYBOARD, int(ke->mKey), ke->mState);
                    if (intent->mInputMap.find(in) != intent->mInputMap.end())
                        intent->mIntents[intent->mInputMap[in]] = true;
				}

				break;
            }

			case EVT_MOUSE_BTN:
            {
				auto me = static_cast<MouseBtnEvent*>(e);

				// Send it across the network
				if (me->mState == BtnState::PRESSED || me->mState == BtnState::RELEASED)
                {
					sf::Packet packet;
					packet << intent->mNetID << EVT_MOUSE_BTN << me->mBtn << me->mState;
					mConn->send(packet, mHndID, 0, 0, false);

					intent->mMouseStates[int(me->mBtn)] = me->mState;

					auto in = IntentComponent::Action(IntentComponent::MOUSE_BTN, int(me->mBtn), me->mState);
					if (intent->mInputMap.find(in) != intent->mInputMap.end())
                        intent->mIntents[intent->mInputMap[in]] = true;
				}

				break;
            }

			case EVT_MOUSE_MOVE:
            {
				auto me = static_cast<MouseMoveEvent*>(e);
				intent->mMouseX, intent->mMouseY = me->mX, me->mY;
				break;
			}
			}
		}
	}

	// ###
	// Packet handling
	/*for _, p := range i.packets[intent->mNetID] {
		var id int
		p.Read(&id)
		switch id {
		case KeyEvent:
			var key input.Key
			var scancode int
			var act input.Action
			var mods input.ModifierKey
			p.Read(&key, &scancode, &act, &mods)

			// Forward controls to other clients
			if mConn->getType() == NetType::SERVER && (act == BtnState::PRESSED || act == BtnState::RELEASED) {
				packet := core.NewOutPacket(nil)
				packet.Write(intent->mNetID, KeyEvent, key, scancode, act, mods)
				mConn->Send(packet, i.hndId, 0, intent->mNetID, false)
			}

			if act == BtnState::PRESSED {
				intent->mKeyStates[int(key)] = BtnState::PRESSED

				in := action{input.StickyKeys, int(key), BtnState::PRESSED}
				if _, ok := intent->mInputMap[in]; ok {
					intent->mIntents[intent->mInputMap[in]] = true
				}
			} else if act == BtnState::RELEASED {
				intent->mKeyStates[int(key)] = BtnState::RELEASED

				in := action{input.StickyKeys, int(key), BtnState::RELEASED}
				if _, ok := intent->mInputMap[in]; ok {
					intent->mIntents[intent->mInputMap[in]] = true
				}
			}
		case MouseBtnEvent:
			var btn input.MouseButton
			var act input.Action
			var mods input.ModifierKey
			p.Read(&btn, &act, &mods)

			// Forward controls to other clients
			if mConn->getType() == NetType::SERVER && (act == BtnState::PRESSED || act == BtnState::RELEASED) {
				packet := core.NewOutPacket(nil)
				packet.Write(intent->mNetID, MouseBtnEvent, btn, act, mods)
				mConn->Send(packet, i.hndId, 0, intent->mNetID, false)
			}

			if act == BtnState::PRESSED {
				intent->mMouseStates[btn] = BtnState::PRESSED

				in := action{input.StickyMouseButtons, btn, BtnState::PRESSED}
				if _, ok := intent->mInputMap[in]; ok {
					intent->mIntents[intent->mInputMap[in]] = true
				}
			} else if act == BtnState::RELEASED {
				intent->mMouseStates[btn] = BtnState::RELEASED

				in := action{input.StickyMouseButtons, btn, BtnState::RELEASED}
				if _, ok := intent->mInputMap[in]; ok {
					intent->mIntents[intent->mInputMap[in]] = true
				}
			}
		case MouseMoveEvent:
			var x, y int
			p.Read(&x, &y)
			intent->mMouseX, intent->mMouseY = x, y
		}
	}*/
}

void IntentSystem::end(const float dt)
{
    for (auto e : mEvents)
        delete e;
    mEvents.clear();

    for (auto& p : mPackets)
        p.second.clear();
}

bool IntentSystem::handleEvent(IEventData const& evt)
{
    if (System::handleEvent(evt))
        return true;

    IEventData *newEvt = NULL;
    switch (evt.getType())
    {
    case EVT_KEY:
        newEvt = new KeyEvent(static_cast<KeyEvent const&>(evt));
        break;
    }

    if (newEvt)
        mEvents.push_back(newEvt);


    return false;
}

void IntentSystem::handlePacket(sf::Packet& packet, int netID)
{
    if (mPackets.find(netID) == mPackets.end())
        mPackets[netID] = std::vector<sf::Packet>();
    mPackets[netID].push_back(packet);
}
