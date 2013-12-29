#include "Fission/Core/EntityManager.h"

#include <algorithm>

#include "Fission/Core/EntityRef.h"

EntityManager::EntityManager(IEventManager* eventManager) : mNextID(0), mEventManager(eventManager)
{
    //ctor
}

EntityManager::~EntityManager()
{
    //dtor
}

EntityRef* EntityManager::createEntity()
{
    int ID;

    if (!mFreeIDs.empty()) // If there's a free ID we can give this entity
    {
        ID = mFreeIDs.back();
        mFreeIDs.pop_back();
    }
    else // Need to make a new ID for this entity
    {
        ID = mNextID;
        mNextID++;

        for (auto componentRow : mComponents)
        {
            componentRow.push_back(NULL);
        }
    }

    EntityRef* entityRef = createEntityRef(ID);
    return entityRef;
}

EntityRef* EntityManager::createEntityRef(int ID)
{
    if (ID >= mNextID || std::find(mFreeIDs.begin(), mFreeIDs.end(), ID) != mFreeIDs.end())
    {
        return new EntityRef(this);
    }

    return new EntityRef(this, ID);
}

void EntityManager::destroyEntity(int ID)
{
    for (auto componentRow : mComponents) // Component arrays, by type
    {
        if (componentRow[ID]) // If the entity has this component type, delete it
        {
            delete componentRow[ID];
            componentRow[ID] = NULL;
        }
    }

    mFreeIDs.push_back(ID); // Free up the entity's ID
}
