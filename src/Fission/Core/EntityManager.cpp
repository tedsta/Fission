#include "Fission/Core/EntityManager.h"

#include <algorithm>

#include "Fission/Core/EntityRef.h"

namespace fsn
{
    // Note: mNextID is zero, because we literally create an entity to set up the NULL entity,
    // whose ID is 0, which increments mNextID to 1, where the real entity IDs start.
    EntityManager::EntityManager(IEventManager* eventManager) : mEventManager(eventManager), mNextID(0)
    {
        createEntity(); // Create the NULL entity.
    }

    EntityManager::~EntityManager()
    {
        //dtor
    }

    int EntityManager::createEntity()
    {
        int ID;

        if (!mFreeIDs.empty()) // If there's a free ID we can give this entity
        {
            ID = mFreeIDs.back();
            mFreeIDs.pop_back();
            mEntityRefs[ID].mID = ID; // Revalidate the EntityRef
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
            mEntityBits.push_back(std::bitset<MaxComponents>());
            mEntityRefs.push_back(EntityRef(this, ID)); // Create a new EntityRef for this entity
        }

        mEntityCount++;

        // Tell the world that this entity has been created.
        mEventManager->fireEvent(EntityEvent(EVENT_CREATE_ENTITY, getEntityRef(ID)));

        return ID;
    }

    EntityRef* EntityManager::getEntityRef(int ID)
    {
        if (!entityExists(ID)) // Entity doesn't exist, return a null EntityRef
        {
            return &mEntityRefs[EntityRef::NULL_ID];
        }

        return &mEntityRefs[ID];
    }

    void EntityManager::destroyEntity(int ID)
    {
        if (!entityExists(ID))
        {
            return;
        }

        // Tell the world that this entity is about to be obliterated.
        mEventManager->fireEvent(EntityEvent(EVENT_DESTROY_ENTITY, getEntityRef(ID)));

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
        mEntityRefs[ID].mID = EntityRef::NULL_ID; // Make the entity NULL.
        mFreeIDs.push_back(ID); // Free up the entity's ID
    }

    bool EntityManager::entityExists(int ID) const
    {
        if (ID == EntityRef::NULL_ID || mNextID <= ID || std::find(mFreeIDs.begin(), mFreeIDs.end(), ID) != mFreeIDs.end())
            return false;

        return true;
    }
}

