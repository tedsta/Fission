#ifndef INTENTSYSTEM_H
#define INTENTSYSTEM_H

#include <map>

#include <Fission/Core/System.h>
#include <Fission/Core/Event.h>
#include <Fission/Network/Connection.h>

namespace fsn
{
    class IEventManager;

    class IntentSystem : public System, public IPacketHandler
    {
        public:
            IntentSystem(IEventManager* eventManager, float lockStep, Connection *conn);
            virtual ~IntentSystem();

            bool handleEvent(const IEventData& evt);
            void handlePacket(sf::Packet& packet, int netID);

        protected:
            /// \brief begin function for systems
            void begin(const float dt);

            /// \brief Process entity function for systems
            void processEntity(EntityRef* entity, const float dt);

            /// \brief end function for systems
            void end(const float dt);

        private:
            Connection* mConn;
            std::vector<IEventData*> mEvents;
            std::map<int, std::vector<sf::Packet>> mPackets;
    };
}


#endif // INTENTSYSTEM_H
