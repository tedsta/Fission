#ifndef NETWORKEVENTS_H_INCLUDED
#define NETWORKEVENTS_H_INCLUDED

#include <SFML/Network/Packet.hpp>

#include <Fission/Core/Event.h>
#include <Fission/Core/EntityEvents.h>
#include <Fission/Input/Events.h>

namespace fsn
{
    enum
    {
        EVT_CONNECT = 200,
        EVT_DISCONNECT
    };

    struct NetworkEvent : public IEventData
    {
        NetworkEvent(EventID evtType, int netID) : IEventData(evtType), mNetID(netID) {}
        int mNetID;
    };
}


#endif // NETWORKEVENTS_H_INCLUDED
