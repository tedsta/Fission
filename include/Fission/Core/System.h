#ifndef SYSTEM_H
#define SYSTEM_H

#include <set>

#include <SFML/System/Thread.hpp>

#include <Fission/Core/Aspect.h>
#include <Fission/Core/Event.h>
#include <Fission/Core/EntityRef.h>

namespace fsn
{
    class IEventManager;

    /// \brief Abstract base class of all entity processing systems.
    class System : public IEventListener
    {
        friend class Engine;

        public:
            System(IEventManager* eventManager, float lockStep);
            virtual ~System();

            /// \brief Event listener event handling callback.
            virtual bool handleEvent(const IEventData& evt);

            /// \brief Get the active entities
            const std::set<EntityRef*>& getActiveEntities(){return mActiveEntities;}

        protected:
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

            /// \brief Get the event manager
            IEventManager* getEventManager() const {return mEventManager;}

            // The aspect for this system. Determines which entities are processed by this system
            // based on their components.
            Aspect mAspect;

        private:
            /// \brief Processes all of the active entities. Used internally.
            void processEntities(const float dt);

            // The event manager
            IEventManager* mEventManager;

            // The active entities this system should process.
            std::set<EntityRef*> mActiveEntities;

            // Used internally for lockstep
            float mLockStep;
            float mDtAccumulator;
    };
}


#endif // SYSTEM_H
