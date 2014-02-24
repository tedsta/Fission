#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <vector>

#include "Fission/Core/EntityManager.h"
#include "Fission/Core/EventManager.h"

namespace fsn
{
    class System;

    /// \brief Core class to facilitate all of the systems and the scene.
    class Engine
    {
        public:
            Engine(float lockStep = 0.f);
            Engine(const fsn::Engine& other); // Copy constructor

            /// \brief Destructor. Stops all system threads, waits for them to successfully
            /// finish, and deallocates everything.
            ~Engine();

            /// \brief Updates all of the systems
            void update(const float dt);

            /// \brief Add a system to this engine.
            /// \note The engine does NOT assume ownership of the system
            void addSystem(System& system);

            /// \brief Gets the event manager.
            EventManager& getEventManager() {return mEventManager;}

            /// \brief Gets the entity manager.
            EntityManager& getEntityManager() {return mEntityManager;}

            /// \brief Gets the lockstep.
            float getLockStep() const {return mLockStep;}

            /// \brief Gets the accumulated delta time
            float getDtAccumulated() const {return mDtAccumulator;}

        private:
            // The event manager
            EventManager mEventManager;

            // The entity manager
            EntityManager mEntityManager;

            // The systems this engine has to manage
            // Engine does not own these pointers
            std::vector<System*> mSystems;

            // The locked time step of the engine
            float mLockStep;

            float mDtAccumulator;
    };
}


#endif // ENGINE_H
