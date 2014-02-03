#ifndef IENTITYOBSERVER_H
#define IENTITYOBSERVER_H

namespace fsn
{
    class Component;
    class EntityRef;

    class IEntityObserver
    {
        public:
            /// \brief Called when an entity is created in the EntityManager.
            virtual void onEntityCreated(EntityRef* entity) = 0;

            /// \brief Called when an entity is destroyed in the EntityManager.
            virtual void onEntityDestroyed(EntityRef* entity) = 0;

            /// \brief Called when a component is added to an entity.
            virtual void onEntityAddedComponent(EntityRef* entity, Component* component) = 0;

            /// \brief Called when a component is removed from an entity.
            virtual void onEntityRemovedComponent(EntityRef* entity, Component* component) = 0;
    };
}


#endif // IENTITYOBSERVER_H