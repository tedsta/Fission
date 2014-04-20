#ifndef FISSION_NETWORKEDENTITYREF_H
#define FISSION_NETWORKEDENTITYREF_H

#include "Fission/Core/EntityObserver.h"
#include "Fission/Core/EntityRef.h"

namespace fsn
{
    class NetworkedEntityRef : public EntityRef, public EntityObserver
    {
        public:
            NetworkedEntityRef();

            void send(Packet& packet);
            void receive(Packet& packet, EntityManager& entityMgr);

            void onEntityCreated(const EntityRef& entity);
            void onEntityDestroyed(const EntityRef& entity);
            void onEntityAddedComponent(const EntityRef& entity, Component& component);
            void onEntityRemovedComponent(const EntityRef& entity, Component& component);

            void operator=(const EntityRef& entity);

        private:
            std::size_t mUniqueID; // Used to receive the entity
    };
}


#endif // FISSION_NETWORKEDENTITYREF_H
