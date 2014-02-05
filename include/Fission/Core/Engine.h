#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <vector>

namespace fsn
{
    class EntityManager;
    class System;
    class IEventManager;

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
            IEventManager* getEventManager() const {return mEventManager.get();}

            /// \brief Gets the entity manager.
            EntityManager* getEntityManager() const {return mEntityManager.get();}

        private:
            // The event manager
            std::unique_ptr<IEventManager> mEventManager;

            // The entity manager
            std::unique_ptr<EntityManager> mEntityManager;

            // The systems this engine has to manage
            // Engine does not own these pointers
            std::vector<System*> mSystems;

            // The locked time step of the engine
            float mLockStep;

            float mDtAccumulator;
    };
}


#endif // ENGINE_H
