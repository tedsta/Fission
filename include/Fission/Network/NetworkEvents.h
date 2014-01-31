#ifndef NETWORKEVENTS_H_INCLUDED
#define NETWORKEVENTS_H_INCLUDED

#include <SFML/Network/Packet.hpp>

#include <Fission/Core/Event.h>

namespace fsn
{
    struct NetworkEvent : public IEventData
    {
        NetworkEvent(int _netID) : netID(_netID) {}
        int netID;
    };
}


#endif // NETWORKEVENTS_H_INCLUDED
