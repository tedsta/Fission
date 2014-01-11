#include <Fission/Core/System.h>

#include <iostream>

#include <SFML/System/Clock.hpp>

#include <Fission/Core/EventManager.h>
#include <Fission/Core/EntityEvents.h>

namespace fsn
{
    System::System(IEventManager* eventManager, float lockStep) : mEventManager(eventManager),
        mLockStep(lockStep), mDtAccumulator(0.f)
    {
        mEventManager->addListener(this, EVENT_CREATE_ENTITY);
        mEventManager->addListener(this, EVENT_DESTROY_ENTITY);
        mEventManager->addListener(this, EVENT_ADD_COMPONENT);
        mEventManager->addListener(this, EVENT_REMOVE_COMPONENT);
    }

    System::~System()
    {
        mEventManager->removeListener(this, EVENT_CREATE_ENTITY);
        mEventManager->removeListener(this, EVENT_DESTROY_ENTITY);
        mEventManager->removeListener(this, EVENT_ADD_COMPONENT);
        mEventManager->removeListener(this, EVENT_REMOVE_COMPONENT);
    }

    bool System::handleEvent(const IEventData& evt)
    {
        switch (evt.getID())
        {
        case EVENT_CREATE_ENTITY:
        case EVENT_ADD_COMPONENT:
        case EVENT_REMOVE_COMPONENT:
            {
                EntityRef* entity = static_cast<const EntityComponentEvent&>(evt).mEntity;

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

                break;
            }

        case EVENT_DESTROY_ENTITY:
            {
                EntityRef* entity = static_cast<const EntityComponentEvent&>(evt).mEntity;
                if (mActiveEntities.find(entity) != mActiveEntities.end())
                {
                    mActiveEntities.erase(entity); // Remove the entity from the active entities if it does not meet the requirements
                    onEntityRemoved(entity);
                }
                break;
            }
        }

        return false;
    }

    void System::processEntities(const float dt)
    {
        // Iterate through the active entities and process each one
        for (EntityRef* entity : mActiveEntities)
        {
            processEntity(entity, dt);
        }
    }
}

