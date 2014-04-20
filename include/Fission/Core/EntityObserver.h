#ifndef FISSION_ENTITYOBSERVER_H
#define FISSION_ENTITYOBSERVER_H

namespace fsn
{
    class Component;
    class EntityRef;

    class EntityObserver
    {
        public:
            /// \brief Called when an entity is created in the EntityManager.
            virtual void onEntityCreated(const EntityRef& entity) = 0;

            /// \brief Called when an entity is destroyed in the EntityManager.
            virtual void onEntityDestroyed(const EntityRef& entity) = 0;

            /// \brief Called when a component is added to an entity.
            virtual void onEntityAddedComponent(const EntityRef& entity, Component& component) = 0;

            /// \brief Called when a component is removed from an entity.
            virtual void onEntityRemovedComponent(const EntityRef& entity, Component& component) = 0;
    };
}


#endif // FISSION_ENTITYOBSERVER_H
