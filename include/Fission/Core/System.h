#ifndef SYSTEM_H
#define SYSTEM_H

#include <set>

#include <Fission/Core/Event.h>
#include <Fission/Core/Entity.h>

class EventManager;

/// \brief Abstract base class of all entity processing systems.
class System : public IEventListener
{
    friend class Engine;
    friend class Scene;

    public:
        System(EventManager *eventManager, float _lockStep, TypeBits typeBits, TypeBits optBits = 0);
        virtual ~System();

        /// \brief Event listener event handling callback.
        virtual bool handleEvent(IEventData const& evt);

        /// \brief Get this system's required type bits
        TypeBits getTypeBits(){return mTypeBits;}

        /// \brief Get this system's optional type bits
        TypeBits getOptBits(){return mOptBits;}

        /// \brief Get the active entities
        const std::set<Entity*>& getActiveEntities(){return mActiveEntities;}

    protected:
        /// \brief Called each time before processing any entities.
        virtual void begin(const float dt) {}

        /// \brief Process a single entity.
        virtual void processEntity(Entity *entity, const float dt) = 0;

        /// \brief Called each time after processing all of the entities.
        virtual void end(const float dt) {}

        /// \brief Get the event manager
        EventManager *getEventManager(){return mEventManager;}

    private:
        /// \brief Processes all of the active entities. Used internally.
        void processEntities(const float dt);

        /// The event manager
        EventManager* const mEventManager;

        /// The type bits an entity must have to be processed by this system.
        const TypeBits mTypeBits;

        /// The optional bits. An entity must have at least one of these to be processed
        TypeBits mOptBits;

        /// The active entities this system should process.
        std::set<Entity*> mActiveEntities;

        // Used by engine for lockstep
        float lockStep;
        float dtAccumulator;
};

#endif // SYSTEM_H
