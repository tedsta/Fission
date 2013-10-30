#include "Core/System.h"

#include <iostream>

#include "Core/EventManager.h"
#include "Core/EntityEvents.h"

System::System(EventManager *eventManager, TypeBits typeBits, TypeBits optBits) : mEventManager(eventManager), mTypeBits(typeBits),
    mOptBits(optBits)
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
    switch (evt.getType())
    {
    case EVENT_ADD_ENTITY:
    case EVENT_ADD_COMPONENT:
    case EVENT_REMOVE_COMPONENT:
        {
            Entity *entity = static_cast<const EntityComponentEvent&>(evt).mEntity;

            // Check if the entity still meets the requirements
            if ((entity->getTypeBits()&mTypeBits) == mTypeBits && (mOptBits == 0 || (entity->getTypeBits()&mOptBits) > 0))
                mActiveEntities.insert(entity); // Add the entity to the active entities if it meets the requirements
            else
                mActiveEntities.erase(entity); // Remove the entity from the active entities if it does not meet the requirements

            break;
        }

    case EVENT_REMOVE_ENTITY:
        {
            Entity *entity = static_cast<const EntityComponentEvent&>(evt).mEntity;
            mActiveEntities.erase(entity); // If the entity was removed from the scene, it definitely won't be active!
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
