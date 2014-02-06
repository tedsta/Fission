#include <Fission/Core/System.h>

#include <algorithm>

#include <SFML/System/Clock.hpp>

#include <Fission/Core/EventManager.h>

namespace fsn
{
    System::System(IEventManager* eventManager) : mEventManager(eventManager)
    {
    }

    System::~System()
    {
    }

    void System::onEntityCreated(const EntityRef& entity)
    {
        checkEntity(entity);
    }

    void System::onEntityDestroyed(const EntityRef& entity)
    {
        auto entityIt = std::find_if(mActiveEntities.begin(), mActiveEntities.end(), EntityRef::find(entity.getID()));
        if (entityIt != mActiveEntities.end())
        {
            mActiveEntities.erase(entityIt); // Remove the entity from the active entities if it does not meet the requirements
            onEntityRemoved(entity);
        }
    }

    void System::onEntityAddedComponent(const EntityRef& entity, Component* component)
    {
        checkEntity(entity);
    }

    void System::onEntityRemovedComponent(const EntityRef& entity, Component* component)
    {
        checkEntity(entity);
    }

    void System::processEntities(const float dt)
    {
        // Iterate through the active entities and process each one
        for (auto& entity : mActiveEntities)
        {
            processEntity(entity, dt);
        }
    }

    void System::checkEntity(const EntityRef& entity)
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

