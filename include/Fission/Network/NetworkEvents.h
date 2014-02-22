#ifndef NETWORKEVENTS_H_INCLUDED
#define NETWORKEVENTS_H_INCLUDED

#include <SFML/Network/Packet.hpp>

#include <Fission/Core/EventManager.h>

namespace fsn
{
    struct NetworkEvent : public EventData
    {
        NetworkEvent(int _netID) : netID(_netID) {}
        int netID;
    };
}


#endif // NETWORKEVENTS_H_INCLUDED
