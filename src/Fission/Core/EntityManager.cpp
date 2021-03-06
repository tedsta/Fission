#include "Fission/Core/EntityManager.h"

#include <algorithm>

#include "Fission/Core/EntityRef.h"

namespace fsn
{
    EntityManager::EntityManager() : mDestructionLocked(false), mNextID(0), mNextUniqueID(1)
    {
    }

    EntityManager::~EntityManager()
    {
        //dtor
    }

    int EntityManager::createEntity(bool giveUniqueID)
    {
        int ID = getNewEntityID();

        if (giveUniqueID)
        {
            mUniqueIDs[ID] = mNextUniqueID++;
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
            return EntityRef(this, EntityRef::NullID);
        }

        return EntityRef(this, ID);
    }

    int EntityManager::getEntityIDFromUniqueID(std::size_t uniqueID)
    {
        if (uniqueID == EntityRef::NullUniqueID)
            return EntityRef::NullID;

        for (int ID = 0; ID < static_cast<int>(mUniqueIDs.size()); ID++)
        {
            if (mUniqueIDs[ID] == uniqueID)
                return ID;
        }

        return EntityRef::NullID; // return null entity
    }

    void EntityManager::destroyEntity(int ID)
    {
        if (!entityExists(ID))
            return;

        if (!mDestructionLocked)
        {
            // Tell the observers that this entity is about to be obliterated
            for (auto observer : mObservers)
                observer->onEntityDestroyed(createEntityRef(ID));

            for (auto& componentRow : mComponents) // Component arrays, by type
            {
                if (componentRow[ID]) // If the entity has this component type, delete it
                {
                    componentRow[ID].reset();
                }
            }

            if (mEntityTags[ID] != -1)
                mTaggedEntities[mEntityTags[ID]].erase(std::find_if(mTaggedEntities[mEntityTags[ID]].begin(),
                                                                 mTaggedEntities[mEntityTags[ID]].end(), EntityRef::find(ID)));

            mEntityCount--;
            mUniqueIDs[ID] = EntityRef::NullUniqueID;
            mEntityBits[ID].reset();
            mEntityTags[ID] = -1;
            mFreeIDs.push_back(ID); // Free up the entity's ID
        }
        else
            mEntitiesToRemove.push_back(ID);

    }

    void EntityManager::serializeEntity(int ID, Packet& packet)
    {
        // The unique ID
        packet << mUniqueIDs[ID];

        // The component count
        packet << static_cast<ComponentType>(mEntityBits[ID].count());

        // The component data
        for (auto& componentRow : mComponents) // Component arrays, by type
        {
            if (componentRow[ID]) // If the entity has this component
            {
                packet << componentRow[ID]->getType();
                componentRow[ID]->serialize(packet);
            }
        }
    }

    int EntityManager::deserializeEntity(Packet& packet)
    {
        // **************************************
        // Create the entity

        int ID = getNewEntityID();
        mEntityCount++;

        packet >> mUniqueIDs[ID];

        // Tell the observers that this entity has been created.
        for (auto observer : mObservers)
            observer->onEntityCreated(createEntityRef(ID));

        // **************************************
        // Deserialize the entity's components

        ComponentType componentCount;
        packet >> componentCount;

        for (std::size_t i = 0; i < static_cast<std::size_t>(componentCount); i++)
        {
            ComponentType type;
            packet >> type;

            addComponentToEntity(ID, type);
            mComponents[type][ID]->deserialize(packet, *this);
        }

        return ID;
    }

    void EntityManager::lockEntityDestruction()
    {
        mDestructionLocked = true;
    }

    void EntityManager::unlockEntityDestruction()
    {
        mDestructionLocked = false;

        for (auto ID : mEntitiesToRemove)
            destroyEntity(ID);
        mEntitiesToRemove.clear();

        for (auto& pair : mComponentsToRemove)
        {
            for (auto componentType : pair.second)
            {
                removeComponentFromEntity(pair.first, componentType);
            }
        }
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

    void EntityManager::removeComponentFromEntity(int ID, ComponentType componentType)
    {
        if (!entityExists(ID))
            return;

        if (componentType >= mComponents.size()) // No entities have this component
            return;

        if (!mComponents[componentType][ID]) // This entity doesn't have this component
            return;

        if (!mDestructionLocked)
        {
            mEntityBits[ID] &= ComponentTypeManager::getBit(componentType).flip(); // Remove the component's bit from the entity's bits.

            // Tell the world that this component's been removed from this entity.
            for (auto observer : mObservers)
                observer->onEntityRemovedComponent(createEntityRef(ID), *mComponents[componentType][ID]);

            mComponents[componentType][ID].reset(); // Delete the component
        }
        else
        {
            if (mComponentsToRemove.find(ID) == mComponentsToRemove.end())
                mComponentsToRemove[ID] = std::vector<ComponentType>();

            mComponentsToRemove[ID].push_back(componentType);
        }
    }

    bool EntityManager::entityHasComponent(int ID, ComponentType componentType)
    {
        if (!entityExists(ID))
            return false;

        if (static_cast<std::size_t>(componentType) < mComponents.size() &&
            mComponents[componentType][ID].get() != nullptr)
            return true;

        return false;
    }

    const std::vector<EntityRef>& EntityManager::getEntitiesWithTag(int tag)
    {
        if (tag >= 0 && static_cast<int>(mTaggedEntities.size()) <= tag)
        {
            mTaggedEntities.resize(tag+1);
        }

        return mTaggedEntities[tag];
    }

    bool EntityManager::entityExists(int ID) const
    {
        if (ID == EntityRef::NullID || ID < 0 || mNextID <= ID || std::find(mFreeIDs.begin(), mFreeIDs.end(), ID) != mFreeIDs.end())
            return false;

        return true;
    }

    void EntityManager::removeEntityObserver(EntityObserver* observer)
    {
        auto it = std::find(mObservers.begin(), mObservers.end(), observer);

        if (it != mObservers.end())
            mObservers.erase(it);
    }

    int EntityManager::getNewEntityID()
    {
        int ID;

        if (!mFreeIDs.empty()) // If there's a free ID we can give this entity
        {
            ID = mFreeIDs.back();
            mFreeIDs.pop_back();
        }
        else // Need to make a new ID for this entity
        {
            ID = mNextID++;

            // Add all of the entity's null components to the component table
            for (auto& componentRow : mComponents)
            {
                componentRow.push_back(NULL);
            }

            // Create the entity bits for this entity.
            mUniqueIDs.push_back(0);
            mEntityBits.push_back(std::bitset<MaxComponents>());
            mEntityTags.push_back(-1);
        }

        return ID;
    }
}

