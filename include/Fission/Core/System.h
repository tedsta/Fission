#ifndef SYSTEM_H
#define SYSTEM_H

#include <set>

#include <SFML/System/Thread.hpp>

#include <Fission/Core/Aspect.h>
#include <Fission/Core/Event.h>
#include <Fission/Core/EntityRef.h>
#include <Fission/Core/IEntityObserver.h>

namespace fsn
{
    class IEventManager;

    /// \brief Abstract base class of all entity processing systems.
    class System : public IEventListener, IEntityObserver
    {
        friend class Engine;

        public:
            System(IEventManager* eventManager);
            virtual ~System();

            /// \brief Called each time before processing any entities.
            virtual void begin(const float dt){}

            /// \brief Process a single entity.
            virtual void processEntity(EntityRef* entity, const float dt) = 0;

            /// \brief Called each time after processing all of the entities.
            virtual void end(const float dt){}

            /// \brief Called when an entity is added to this system
            virtual void onEntityAdded(EntityRef* entity){}

            /// \brief Called when an entity is removed from this system
            virtual void onEntityRemoved(EntityRef* entity){}

            // Getters

            /// \brief Get the event manager
            IEventManager* getEventManager() const {return mEventManager;}

            /// \brief Get the active entities
            const std::set<EntityRef*>& getActiveEntities(){return mActiveEntities;}

            // Entity callbacks

            void onEntityCreated(EntityRef* entity);
            void onEntityDestroyed(EntityRef* entity);
            void onEntityAddedComponent(EntityRef* entity, Component* component);
            void onEntityRemovedComponent(EntityRef* entity, Component* component);

            // The aspect for this system. Determines which entities are processed by this system
            // based on their components.
            // TODO: I wish this wasn't public
            Aspect mAspect;

        private:
            /// \brief Processes all of the active entities. Used internally.
            void processEntities(const float dt);

            /// \brief Checks if an entity should be processed by this system and adds or removes
            /// it from this system as necessary.
            void checkEntity(EntityRef* entity);

            // The event manager
            IEventManager* mEventManager;

            // The active entities this system should process.
            std::set<EntityRef*> mActiveEntities;
    };
}


#endif // SYSTEM_H
