#include "Fission/Core/EntityRef.h"

namespace fsn
{
    EntityRef::EntityRef() : mEntityManager(nullptr), mID(NullID)
    {
        //ctor
    }

    EntityRef::EntityRef(EntityManager* em, int ID) : mEntityManager(em), mID(ID)
    {
        //ctor
    }

    bool EntityRef::exists() const
    {
        if (mEntityManager)
            return mEntityManager->entityExists(mID);
        return false;
    }

    void EntityRef::destroy() const
    {
        if (mEntityManager)
            mEntityManager->destroyEntity(mID);
    }

    void EntityRef::serialize(Packet& packet) const
    {
        mEntityManager->serializeEntity(mID, packet);
    }

    void EntityRef::setTag(int tag) const
    {
        if (mEntityManager)
            mEntityManager->setEntityTag(mID, tag);
    }

    std::size_t EntityRef::getUniqueID() const
    {
        if (mEntityManager)
            return mEntityManager->getUniqueEntityID(mID);
        return NullUniqueID;
    }

    const std::bitset<MaxComponents>& EntityRef::getBits() const
    {
        return mEntityManager->getEntityBits(mID);
    }

    bool EntityRef::operator==(const EntityRef& other) const
    {
        return mID==other.mID && mEntityManager==other.mEntityManager;
    }

    bool EntityRef::operator!=(const EntityRef& other) const
    {
        return mID!=other.mID || mEntityManager!=other.mEntityManager;
    }
}

