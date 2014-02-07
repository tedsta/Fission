#include "Fission/Core/Systems/ComponentSystem.h"

#include <algorithm>

namespace fsn
{
    ComponentSystem::ComponentSystem(EntityManager& entityMgr)
    {
        entityMgr.addEntityObserver(this);
    }

    void ComponentSystem::update(const float dt)
    {
        begin(dt);

        // Iterate through the active entities and process each one
        for (auto& entity : mActiveEntities)
        {
            processEntity(entity, dt);
        }

        end(dt);
    }

    void ComponentSystem::onEntityCreated(const EntityRef& entity)
    {
        checkEntity(entity);
    }

    void ComponentSystem::onEntityDestroyed(const EntityRef& entity)
    {
        auto entityIt = std::find_if(mActiveEntities.begin(), mActiveEntities.end(), EntityRef::find(entity.getID()));
        if (entityIt != mActiveEntities.end())
        {
            mActiveEntities.erase(entityIt); // Remove the entity from the active entities if it does not meet the requirements
            onEntityRemoved(entity);
        }
    }

    void ComponentSystem::onEntityAddedComponent(const EntityRef& entity, Component* component)
    {
        checkEntity(entity);
    }

    void ComponentSystem::onEntityRemovedComponent(const EntityRef& entity, Component* component)
    {
        checkEntity(entity);
    }

    void ComponentSystem::checkEntity(const EntityRef& entity)
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
        else if (!mAspect.checkEntity(entity))
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
