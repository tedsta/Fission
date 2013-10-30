#ifndef NETWORKEVENTS_H_INCLUDED
#define NETWORKEVENTS_H_INCLUDED

#include <SFML/Network/Packet.hpp>

#include "Core/Event.h"
#include "Core/EntityEvents.h"
#include "Input/Events.h"

enum
{
    EVT_CONNECT = 200,
    EVT_DISCONNECT
};

struct NetworkEvent : public IEventData
{
    NetworkEvent(EventType evtType, int netID) : IEventData(evtType), mNetID(netID) {}
    int mNetID;
};

#endif // NETWORKEVENTS_H_INCLUDED
