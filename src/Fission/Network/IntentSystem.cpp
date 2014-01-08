#include <Fission/Network/IntentSystem.h>

#include <Fission/Core/EventManager.h>
#include <Fission/Input/Defs.h>
#include <Fission/Input/Events.h>
#include <Fission/Network/Connection.h>
#include <Fission/Network/Intent.h>
#include <Fission/Network/NetworkEvents.h>

#include <iostream>

namespace fsn
{
    IntentSystem::IntentSystem(IEventManager* eventManager, float lockStep, Connection *conn) : System(eventManager, lockStep),
        mConn(conn)
    {
        mAspect.all<Intent>();
        mConn->registerHandlerAuto(this);
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

    void IntentSystem::processEntity(EntityRef* entity, const float dt)
    {
        auto intent = entity->getComponent<Intent>();

        for (auto& it : intent->mIntents)
            it.second = false;

        for (int k = 0; k < sf::Keyboard::KeyCount; k++)
        {
            if (intent->mKeyStates[k] == Pressed)
                intent->mKeyStates[k] = Down;
            else if (intent->mKeyStates[k] == Released)
                intent->mKeyStates[k] = Up;

            auto in = Intent::Action(Intent::KEYBOARD, k, intent->mKeyStates[k]);

            if (intent->mInputMap.find(in) != intent->mInputMap.end())
            {
                intent->mIntents[intent->mInputMap[in]] = true;
            }
        }

        for (int m = 0; m < sf::Mouse::ButtonCount; m++)
        {
            if (intent->mMouseStates[m] == Pressed)
                intent->mMouseStates[m] = Down;
            else if (intent->mMouseStates[m] == Released)
                intent->mMouseStates[m] = Up;

            auto in = Intent::Action(Intent::MOUSE_BTN, m, intent->mMouseStates[m]);

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
                    if (ke->mState == Pressed || ke->mState == Released)
                    {
                        sf::Packet packet;
                        packet << intent->mNetID << int(EVT_KEY) << int(ke->mKey) << ke->mState;
                        mConn->send(packet, getHndID(), 0, 0, false);

                        intent->mKeyStates[int(ke->mKey)] = ke->mState;

                        auto in = Intent::Action(Intent::KEYBOARD, int(ke->mKey), ke->mState);
                        if (intent->mInputMap.find(in) != intent->mInputMap.end())
                            intent->mIntents[intent->mInputMap[in]] = true;
                    }

                    break;
                }

                case EVT_MOUSE_BTN:
                {
                    auto me = static_cast<MouseBtnEvent*>(e);

                    // Send it across the network
                    if (me->mState == Pressed || me->mState == Released)
                    {
                        sf::Packet packet;
                        packet << intent->mNetID << int(EVT_MOUSE_BTN) << int(me->mBtn) << me->mState;
                        mConn->send(packet, getHndID(), 0, 0, false);

                        intent->mMouseStates[int(me->mBtn)] = me->mState;

                        auto in = Intent::Action(Intent::MOUSE_BTN, int(me->mBtn), me->mState);
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
                    mConn->send(packet, getHndID(), 0, 0, false);

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

                if (state == Pressed || state == Released)
                {
                    // Forward controls to other clients
                    if (mConn->getType() == NetType::SERVER)
                    {
                        sf::Packet packet;
                        packet << intent->mNetID << int(EVT_KEY) << key << state;
                        mConn->send(packet, getHndID(), 0, intent->mNetID, false);
                    }

                    intent->mKeyStates[int(key)] = state;

                    auto in = Intent::Action(Intent::KEYBOARD, key, state);
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

                if (state == Pressed || state == Released)
                {
                    // Forward controls to other clients
                    if (mConn->getType() == NetType::SERVER)
                    {
                        sf::Packet packet;
                        packet << intent->mNetID << int(EVT_MOUSE_BTN) << int(btn) << state;
                        mConn->send(packet, getHndID(), 0, intent->mNetID, false);
                    }

                    intent->mMouseStates[int(btn)] = state;

                    auto in = Intent::Action(Intent::MOUSE_BTN, int(btn), state);
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

    bool IntentSystem::handleEvent(const IEventData& evt)
    {
        if (System::handleEvent(evt))
            return true;

        IEventData *newEvt = NULL;
        switch (evt.getID())
        {
        case EVT_KEY:
            newEvt = new KeyEvent(static_cast<const KeyEvent&>(evt));
            break;
        case EVT_MOUSE_BTN:
            newEvt = new MouseBtnEvent(static_cast<const MouseBtnEvent&>(evt));
            break;
        case EVT_MOUSE_MOVE:
            newEvt = new MouseMoveEvent(static_cast<const MouseMoveEvent&>(evt));
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
}

