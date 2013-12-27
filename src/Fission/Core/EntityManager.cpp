#include "Fission/Core/EntityManager.h"

#include "Fission/Core/EntityRef.h"

EntityManager::EntityManager() : mNextID(0)
{
    //ctor
}

EntityManager::~EntityManager()
{
    //dtor
}

EntityRef* EntityManager::createEntity()
{
    if (!mFreeIDs.empty())
    {
    }
    else
    {
        int ID = mNextID;
        mNextID++;
        EntityRef* entityRef = createEntityRef(ID);
    }
}

EntityRef* EntityManager::createEntityRef(int ID)
{
    if (ID >= mNextID || mFreeIDs.find(ID) != mFreeIDs.end())
    {
        return new EntityRef(this);
    }

    return new EntityRef(this, ID);
}
