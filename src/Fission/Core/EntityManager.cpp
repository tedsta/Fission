#include "Fission/Core/EntityManager.h"

#include <algorithm>

#include "Fission/Core/EntityRef.h"

EntityManager::EntityManager(IEventManager* eventManager) : mEventManager(eventManager), mNextID(0)
{
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

        // Add all of the entity's null components to the component table
        for (auto& componentRow : mComponents)
        {
            componentRow.push_back(NULL);
        }

        // Create the entity bits for this entity.
        mEntityBits.push_back(std::bitset<MAX_COMPONENTS>());
    }

    mEntityCount++;

    EntityRef* entityRef = createEntityRef(ID);
    return entityRef;
}

EntityRef* EntityManager::createEntityRef(int ID)
{
    if (!entityExists(ID)) // Entity doesn't exist, return a null EntityRef
    {
        return new EntityRef(this);
    }

    return new EntityRef(this, ID);
}

void EntityManager::destroyEntity(int ID)
{
    for (auto& componentRow : mComponents) // Component arrays, by type
    {
        if (componentRow[ID]) // If the entity has this component type, delete it
        {
            delete componentRow[ID];
            componentRow[ID] = NULL;
        }
    }

    mEntityCount--;
    mEntityBits[ID].reset();
    mFreeIDs.push_back(ID); // Free up the entity's ID
}

bool EntityManager::entityExists(int ID) const
{
    if (ID == EntityRef::NULL_ID || mNextID <= ID || std::find(mFreeIDs.begin(), mFreeIDs.end(), ID) != mFreeIDs.end())
        return false;

    return true;
}
