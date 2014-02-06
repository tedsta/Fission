#include "Fission/Core/EntityManager.h"

#include <algorithm>

#include "Fission/Core/EntityRef.h"

namespace fsn
{
    EntityManager::EntityManager() : mNextID(0)
    {
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
            mEntityTags.push_back(-1);
        }

        mEntityCount++;

        // Tell the observers that this entity has been created.
        for (auto observer : mObservers)
            observer->onEntityCreated(createEntityRef(ID));

        return ID;
    }

    EntityRef EntityManager::createEntityRef(int ID)
    {
        if (!entityExists(ID)) // Entity doesn't exist, return a null EntityRef
        {
            return EntityRef(this, EntityRef::NULL_ID);
        }

        return EntityRef(this, ID);
    }

    void EntityManager::destroyEntity(int ID)
    {
        if (!entityExists(ID))
            return;

        // Tell the observers that this entity is about to be obliterated
        for (auto observer : mObservers)
            observer->onEntityDestroyed(createEntityRef(ID));

        for (auto& componentRow : mComponents) // Component arrays, by type
        {
            if (componentRow[ID]) // If the entity has this component type, delete it
            {
                delete componentRow[ID];
                componentRow[ID] = NULL;
            }
        }

        if (mEntityTags[ID] != -1)
            mTaggedEntities[mEntityTags[ID]].erase(std::find_if(mTaggedEntities[mEntityTags[ID]].begin(),
                                                             mTaggedEntities[mEntityTags[ID]].end(), EntityRef::find(ID)));

        mEntityCount--;
        mEntityBits[ID].reset();
        mEntityTags[ID] = -1;
        mFreeIDs.push_back(ID); // Free up the entity's ID
    }

    void EntityManager::setEntityTag(int ID, int tag)
    {
        if (!entityExists(ID))
            return;

        // Remove the entity from its old tag list
        if (mEntityTags[ID] != -1)
            mTaggedEntities[mEntityTags[ID]].erase(std::find_if(mTaggedEntities[mEntityTags[ID]].begin(),
                                                             mTaggedEntities[mEntityTags[ID]].end(), EntityRef::find(ID)));

        mEntityTags[ID] = tag;

        if (tag != -1)
        {
            if (static_cast<int>(mTaggedEntities.size()) <= tag)
                mTaggedEntities.resize(tag+1);

            mTaggedEntities[tag].push_back(createEntityRef(ID));
        }
    }

    bool EntityManager::entityExists(int ID) const
    {
        if (ID == EntityRef::NULL_ID || ID < 0 || mNextID <= ID || std::find(mFreeIDs.begin(), mFreeIDs.end(), ID) != mFreeIDs.end())
            return false;

        return true;
    }
}

