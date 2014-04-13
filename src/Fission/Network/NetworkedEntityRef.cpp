#include "Fission/Network/NetworkedEntityRef.h"

namespace fsn
{
    NetworkedEntityRef::NetworkedEntityRef()
    {
        //ctor
    }

    void NetworkedEntityRef::send(Packet& packet)
    {
        packet << getUniqueID();
    }

    void NetworkedEntityRef::receive(Packet& packet, EntityManager& entityMgr)
    {
        packet >> mUniqueID;

        mEntityManager = &entityMgr;
        mID = mEntityManager->getEntityIDFromUniqueID(mUniqueID);

        if (mID == EntityRef::NullID)
        {
            mEntityManager->addEntityObserver(this);
        }
    }

    void NetworkedEntityRef::onEntityCreated(const EntityRef& entity)
    {
        if (entity.getUniqueID() == mUniqueID)
        {
            // At last our search is over :)
            mID = entity.getID();
            mEntityManager->removeEntityObserver(this);
        }
    }

    void NetworkedEntityRef::onEntityDestroyed(const EntityRef& entity)
    {
        // Don't care
    }

    void NetworkedEntityRef::onEntityAddedComponent(const EntityRef& entity, Component& component)
    {
        // Don't care
    }

    void NetworkedEntityRef::onEntityRemovedComponent(const EntityRef& entity, Component& component)
    {
        // Don't care
    }

    void NetworkedEntityRef::operator=(const EntityRef& entity)
    {
        mEntityManager = entity.mEntityManager;
        mID = entity.mID;
    }
}

