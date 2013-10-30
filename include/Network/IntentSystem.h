#ifndef INTENTSYSTEM_H
#define INTENTSYSTEM_H

#include <map>

#include "Core/System.h"
#include "Core/Event.h"
#include "Network/Connection.h"

class EventManager;

class IntentSystem : public System, public IPacketHandler
{
    public:
        IntentSystem(EventManager *eventManager, Connection *conn);
        virtual ~IntentSystem();

        bool handleEvent(IEventData const& evt);
        void handlePacket(sf::Packet& packet, int netID);

    protected:
        /// \brief begin function for systems
        void begin(const float dt);

        /// \brief Process entity function for systems
        void processEntity(Entity* entity, const float dt);

        /// \brief end function for systems
        void end(const float dt);

    private:
        Connection* mConn;
        std::vector<IEventData*> mEvents;
        std::map<int, std::vector<sf::Packet>> mPackets;
        int mHndID;
};

#endif // INTENTSYSTEM_H
