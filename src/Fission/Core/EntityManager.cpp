#include "Fission/Core/EntityManager.h"

#include <algorithm>

#include "Fission/Core/EntityRef.h"

namespace fsn
{
    // Note: mNextID is zero, because we literally create an entity to set up the NULL entity,
    // whose ID is 0, which increments mNextID to 1, where the real entity IDs start.
    EntityManager::EntityManager() : mNextID(0)
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
            mEntityRefs[ID]->mID = ID; // Revalidate the EntityRef
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
            mEntityRefs.push_back(new EntityRef(*this, ID)); // Create a new EntityRef for this entity
            mEntityTags.push_back(-1);
        }

        mEntityCount++;

        // Tell the observers that this entity has been created.
        for (auto observer : mObservers)
            observer->onEntityCreated(getEntityRef(ID));

        return ID;
    }

    EntityRef* EntityManager::getEntityRef(int ID)
    {
        if (!entityExists(ID)) // Entity doesn't exist, return a null EntityRef
        {
            return mEntityRefs[EntityRef::NULL_ID];
        }

        return mEntityRefs[ID];
    }

    void EntityManager::destroyEntity(int ID)
    {
        auto entity = getEntityRef(ID);
        if (entity->getID() == EntityRef::NULL_ID)
            return;

        // Tell the observers that this entity is about to be obliterated
        for (auto observer : mObservers)
            observer->onEntityDestroyed(getEntityRef(ID));

        for (auto& componentRow : mComponents) // Component arrays, by type
        {
            if (componentRow[ID]) // If the entity has this component type, delete it
            {
                delete componentRow[ID];
                componentRow[ID] = NULL;
            }
        }

        if (mEntityTags[ID] != -1)
            mTaggedEntities[mEntityTags[ID]].erase(std::find(mTaggedEntities[mEntityTags[ID]].begin(),
                                                             mTaggedEntities[mEntityTags[ID]].end(), entity));

        mEntityCount--;
        mEntityBits[ID].reset();
        mEntityTags[ID] = -1;
        mEntityRefs[ID]->mID = EntityRef::NULL_ID; // Make the entity NULL.
        mFreeIDs.push_back(ID); // Free up the entity's ID
    }

    void EntityManager::setEntityTag(int ID, int tag)
    {
        auto entity = getEntityRef(ID);
        if (entity->getID() == EntityRef::NULL_ID)
            return;

        if (mEntityTags[ID] != -1)
            mTaggedEntities[mEntityTags[ID]].erase(std::find(mTaggedEntities[mEntityTags[ID]].begin(),
                                                             mTaggedEntities[mEntityTags[ID]].end(), entity));

        mEntityTags[ID] = tag;

        if (tag != -1)
        {
            if (static_cast<int>(mTaggedEntities.size()) <= tag)
                mTaggedEntities.resize(tag+1);

            mTaggedEntities[tag].push_back(entity);
        }
    }

    bool EntityManager::entityExists(int ID) const
    {
        if (ID == EntityRef::NULL_ID || ID < 0 || mNextID <= ID || std::find(mFreeIDs.begin(), mFreeIDs.end(), ID) != mFreeIDs.end())
            return false;

        return true;
    }
}

