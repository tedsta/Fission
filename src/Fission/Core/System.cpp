#include <Fission/Core/System.h>

#include <iostream>

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

    void System::onEntityCreated(EntityRef* entity)
    {
        checkEntity(entity);
    }

    void System::onEntityDestroyed(EntityRef* entity)
    {
        if (mActiveEntities.find(entity) != mActiveEntities.end())
        {
            mActiveEntities.erase(entity); // Remove the entity from the active entities if it does not meet the requirements
            onEntityRemoved(entity);
        }
    }

    void System::onEntityAddedComponent(EntityRef* entity, Component* component)
    {
        checkEntity(entity);
    }

    void System::onEntityRemovedComponent(EntityRef* entity, Component* component)
    {
        checkEntity(entity);
    }

    void System::processEntities(const float dt)
    {
        // Iterate through the active entities and process each one
        for (EntityRef* entity : mActiveEntities)
        {
            processEntity(entity, dt);
        }
    }

    void System::checkEntity(EntityRef* entity)
    {
        // Check if the entity still meets the requirements
        if (mAspect.checkEntity(entity))
        {
            if (mActiveEntities.find(entity) == mActiveEntities.end())
            {
                mActiveEntities.insert(entity); // Add the entity to the active entities if it meets the requirements
                onEntityAdded(entity);
            }
        }
        else if (!mAspect.checkEntity(entity))
        {
            if (mActiveEntities.find(entity) != mActiveEntities.end())
            {
                mActiveEntities.erase(entity); // Remove the entity from the active entities if it does not meet the requirements
                onEntityRemoved(entity);
            }
        }
    }
}

