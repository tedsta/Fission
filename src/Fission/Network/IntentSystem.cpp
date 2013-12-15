#include <Fission/Network/IntentSystem.h>

#include <Fission/Core/EventManager.h>
#include <Fission/Input/Defs.h>
#include <Fission/Input/Events.h>
#include <Fission/Network/Connection.h>
#include <Fission/Network/IntentComponent.h>
#include <Fission/Network/NetworkEvents.h>

#include <iostream>

IntentSystem::IntentSystem(EventManager *eventManager, float lockStep, Connection *conn) : System(eventManager, lockStep, IntentComponent::Type), mConn(conn)
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
			switch (e->getID())
			{
			case EVT_KEY:
            {
                auto ke = static_cast<KeyEvent*>(e);

				// Send it across the network
				if (ke->mState == BtnState::PRESSED || ke->mState == BtnState::RELEASED)
                {
					sf::Packet packet;
					packet << intent->mNetID << int(EVT_KEY) << int(ke->mKey) << ke->mState;
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
					packet << intent->mNetID << int(EVT_MOUSE_BTN) << int(me->mBtn) << me->mState;
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
				intent->mMousePos.x = me->mX;
				intent->mMousePos.y = me->mY;

				sf::Packet packet;
				packet << intent->mNetID << int(EVT_MOUSE_MOVE) << intent->mMousePos.x << intent->mMousePos.y;
				mConn->send(packet, mHndID, 0, 0, false);

				break;
			}
			}
		}
	}

	// ###
	// Packet handling
	for (auto& packet : mPackets[intent->mNetID])
	{
	    int netID;
		int id;
		packet >> netID >> id;
		switch (id)
		{
		case EVT_KEY:
        {
			int key;
			int state;
			packet >> key >> state;

			if (state == BtnState::PRESSED || state == BtnState::RELEASED)
            {
                // Forward controls to other clients
                if (mConn->getType() == NetType::SERVER)
                {
                    sf::Packet packet;
                    packet << intent->mNetID << int(EVT_KEY) << key << state;
                    mConn->send(packet, mHndID, 0, intent->mNetID, false);
                }

                intent->mKeyStates[int(key)] = state;

                auto in = IntentComponent::Action(IntentComponent::KEYBOARD, key, state);
                if (intent->mInputMap.find(in) != intent->mInputMap.end())
                    intent->mIntents[intent->mInputMap[in]] = true;
            }
			break;
        }

		case EVT_MOUSE_BTN:
        {
			int btn;
			int state;
			packet >> btn >> state;

			if (state == BtnState::PRESSED || state == BtnState::RELEASED)
            {
                // Forward controls to other clients
                if (mConn->getType() == NetType::SERVER)
                {
                    sf::Packet packet;
                    packet << intent->mNetID << int(EVT_MOUSE_BTN) << int(btn) << state;
                    mConn->send(packet, mHndID, 0, intent->mNetID, false);
                }

                intent->mMouseStates[int(btn)] = state;

                auto in = IntentComponent::Action(IntentComponent::MOUSE_BTN, int(btn), state);
                if (intent->mInputMap.find(in) != intent->mInputMap.end())
                    intent->mIntents[intent->mInputMap[in]] = true;
            }
			break;
        }

		case EVT_MOUSE_MOVE:
        {
			packet >> intent->mMousePos.x >> intent->mMousePos.y;
			break;
        }
		}
	}
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
    switch (evt.getID())
    {
    case EVT_KEY:
        newEvt = new KeyEvent(static_cast<KeyEvent const&>(evt));
        break;
    case EVT_MOUSE_BTN:
        newEvt = new MouseBtnEvent(static_cast<MouseBtnEvent const&>(evt));
        break;
    case EVT_MOUSE_MOVE:
        newEvt = new MouseMoveEvent(static_cast<MouseMoveEvent const&>(evt));
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
