#ifndef __EVENT_TYPE_H__
#define __EVENT_TYPE_H__

#include "Core/Event.h"

class Entity;
class Component;

enum
{
    EVENT_ADD_ENTITY,
    EVENT_REMOVE_ENTITY,
    EVENT_ADD_COMPONENT,
    EVENT_REMOVE_COMPONENT,

    CUSTOM_EVENT = 1000
};

struct EntityEvent : public IEventData
{
    EntityEvent(EventType evtType, Entity *e) : IEventData(evtType), mEntity(e) {}
    Entity *mEntity;
};

struct EntityComponentEvent : public IEventData
{
    EntityComponentEvent(EventType evtType, Entity *e, Component *c)
        : IEventData(evtType), mEntity(e), mComponent(c) {}

    Entity *mEntity;
    Component *mComponent;
};

#endif
