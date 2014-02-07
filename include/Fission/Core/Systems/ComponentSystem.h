#ifndef COMPONENTSYSTEM_H
#define COMPONENTSYSTEM_H

#include <vector>

#include "Fission/Core/Aspect.h"
#include "Fission/Core/EntityRef.h"
#include "Fission/Core/IEntityObserver.h"
#include "Fission/Core/Systems/System.h"

namespace fsn
{
    class ComponentSystem : public System, public IEntityObserver
    {
        public:
            ComponentSystem(EntityManager& entityMgr);

            /// \brief Called each time before processing any entities.
            virtual void begin(const float dt) {}

            /// \brief Process a single entity.
            virtual void processEntity(const EntityRef& entity, const float dt) = 0;

            /// \brief Called each time after processing all of the entities.
            virtual void end(const float dt) {}

            /// \brief Called when an entity is added to this system
            virtual void onEntityAdded(const EntityRef& entity){}

            /// \brief Called when an entity is removed from this system
            virtual void onEntityRemoved(const EntityRef& entity){}

            /// \brief Get the active entities
            const std::vector<EntityRef>& getActiveEntities(){return mActiveEntities;}

            /// \brief Engine uses this internally to update the system.
            void update(const float dt);

            // Entity callbacks

            void onEntityCreated(const EntityRef& entity);
            void onEntityDestroyed(const EntityRef& entity);
            void onEntityAddedComponent(const EntityRef& entity, Component* component);
            void onEntityRemovedComponent(const EntityRef& entity, Component* component);

        protected:
            // The aspect for this system. Determines which entities are processed by this system
            // based on their components.
            // TODO: Figure out a better way to modify the aspect of systems
            Aspect mAspect;

        private:
            /// \brief Checks if an entity should be processed by this system and adds or removes
            /// it from this system as necessary.
            void checkEntity(const EntityRef& entity);

            // The active entities this system should process.
            std::vector<EntityRef> mActiveEntities;
    };
}

#endif // COMPONENTSYSTEM_H
