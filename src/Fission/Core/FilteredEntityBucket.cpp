#include "Fission\Core\FilteredEntityBucket.h"

#include <algorithm>

namespace fsn
{
    FilteredEntityBucket::FilteredEntityBucket()
    {
        //ctor
    }

    void FilteredEntityBucket::onEntityCreated(const EntityRef& entity)
    {
        checkEntity(entity);
    }

    void FilteredEntityBucket::onEntityDestroyed(const EntityRef& entity)
    {
        auto entityIt = std::find_if(mActiveEntities.begin(), mActiveEntities.end(), EntityRef::find(entity.getID()));
        if (entityIt != mActiveEntities.end())
        {
            mActiveEntities.erase(entityIt); // Remove the entity from the active entities if it does not meet the requirements
            onEntityRemoved(entity);
        }
    }

    void FilteredEntityBucket::onEntityAddedComponent(const EntityRef& entity, Component& component)
    {
        checkEntity(entity);
    }

    void FilteredEntityBucket::onEntityRemovedComponent(const EntityRef& entity, Component& component)
    {
        checkEntity(entity);
    }

    void FilteredEntityBucket::checkEntity(const EntityRef& entity)
    {
        // Check if the entity still meets the requirements
        if (mAspect.checkEntity(entity))
        {
            auto entityIt = std::find_if(mActiveEntities.begin(), mActiveEntities.end(), EntityRef::find(entity.getID()));
            if (entityIt == mActiveEntities.end())
            {
                mActiveEntities.push_back(entity); // Add the entity to the active entities if it meets the requirements
                onEntityAdded(entity);
            }
        }
        else
        {
            auto entityIt = std::find_if(mActiveEntities.begin(), mActiveEntities.end(), EntityRef::find(entity.getID()));
            if (entityIt != mActiveEntities.end())
            {
                mActiveEntities.erase(entityIt); // Remove the entity from the active entities if it does not meet the requirements
                onEntityRemoved(entity);
            }
        }
    }
}

