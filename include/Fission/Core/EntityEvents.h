#ifndef __EVENT_TYPE_H__
#define __EVENT_TYPE_H__

#include <Fission/Core/Event.h>

namespace fsn
{
    class EntityRef;
    class Component;

    enum
    {
        EVENT_CREATE_ENTITY,
        EVENT_DESTROY_ENTITY,
        EVENT_ADD_COMPONENT,
        EVENT_REMOVE_COMPONENT,

        CUSTOM_EVENT = 1000
    };

    struct EntityEvent : public IEventData
    {
        EntityEvent(EventID evtType, EntityRef *e) : IEventData(evtType), mEntity(e) {}
        EntityRef* mEntity;
    };

    struct EntityComponentEvent : public IEventData
    {
        EntityComponentEvent(EventID evtType, EntityRef *e, Component *c)
            : IEventData(evtType), mEntity(e), mComponent(c) {}

        EntityRef* mEntity;
        Component* mComponent;
    };
}


#endif
