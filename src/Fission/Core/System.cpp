#include <Fission/Core/System.h>

#include <iostream>

#include <Fission/Core/EventManager.h>
#include <Fission/Core/EntityEvents.h>

System::System(EventManager *eventManager, float _lockStep, TypeBits typeBits, TypeBits optBits) : mEventManager(eventManager),
    mTypeBits(typeBits), lockStep(_lockStep), dtAccumulator(0.f), mOptBits(optBits)
{
    mEventManager->addListener(this, EVENT_ADD_ENTITY);
    mEventManager->addListener(this, EVENT_REMOVE_ENTITY);
    mEventManager->addListener(this, EVENT_ADD_COMPONENT);
    mEventManager->addListener(this, EVENT_REMOVE_COMPONENT);
}

System::~System()
{
    mEventManager->removeListener(this, EVENT_ADD_ENTITY);
    mEventManager->removeListener(this, EVENT_REMOVE_ENTITY);
    mEventManager->removeListener(this, EVENT_ADD_COMPONENT);
    mEventManager->removeListener(this, EVENT_REMOVE_COMPONENT);
}

bool System::handleEvent(IEventData const& evt)
{
    switch (evt.getID())
    {
    case EVENT_ADD_ENTITY:
    case EVENT_ADD_COMPONENT:
    case EVENT_REMOVE_COMPONENT:
        {
            Entity *entity = static_cast<const EntityComponentEvent&>(evt).mEntity;

            // Check if the entity still meets the requirements
            if ((entity->getTypeBits()&mTypeBits) == mTypeBits && (mOptBits == 0 || (entity->getTypeBits()&mOptBits) > 0))
            {
                if (mActiveEntities.find(entity) == mActiveEntities.end())
                {
                    mActiveEntities.insert(entity); // Add the entity to the active entities if it meets the requirements
                    onEntityAdded(entity);
                }
            }
            else if (mActiveEntities.find(entity) != mActiveEntities.end())
            {
                mActiveEntities.erase(entity); // Remove the entity from the active entities if it does not meet the requirements
                onEntityRemoved(entity);
            }

            break;
        }

    case EVENT_REMOVE_ENTITY:
        {
            Entity *entity = static_cast<const EntityComponentEvent&>(evt).mEntity;
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
    for (Entity *entity : mActiveEntities)
    {
        processEntity(entity, dt);
    }
}
