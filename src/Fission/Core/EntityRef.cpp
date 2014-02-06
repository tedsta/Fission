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

    void EntityRef::setTag(int tag) const
    {
        if (mEntityManager)
            mEntityManager->setEntityTag(mID, tag);
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

