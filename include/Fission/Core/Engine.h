#ifndef ENGINE_H
#define ENGINE_H

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
            Engine();

            /// \brief Destructor. Stops all system threads, waits for them to successfully
            /// finish, and deallocates everything.
            ~Engine();

            /// \brief Updates all of the systems
            void update(const float dt);

            /// \brief Add a system to this engine.
            void addSystem(System *system){mSystems.push_back(system);}

            /// \brief Gets the event manager.
            IEventManager* getEventManager() const {return mEventManager;}

            /// \brief Gets the entity manager.
            EntityManager* getEntityManager() const {return mEntityManager;}

        private:
            // The event manager
            IEventManager* mEventManager;

            // The entity manager
            EntityManager* mEntityManager;

            // The systems this engine has to manage
            std::vector<System*> mSystems;
    };
}


#endif // ENGINE_H
