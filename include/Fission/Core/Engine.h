#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

class Scene;
class System;
class EventManager;

/// \brief Core class to facilitate all of the systems and the scene.
class Engine
{
    public:
        Engine();
        virtual ~Engine();

        /// \brief Updates all of the systems in this engine.
        void update(const float dt);

        /// \brief Add a system to this engine.
        void addSystem(System *system){mSystems.push_back(system);}

        /// \brief Gets the event manager.
        EventManager *getEventManager() const {return mEventManager;}

        /// \brief Gets the current scene.
        Scene *getScene() const {return mScene;}

    private:
        /// The event manager
        EventManager *mEventManager;

        /// The current scene
        Scene *mScene;

        /// The systems this engine has to manage
        std::vector<System*> mSystems;
};

#endif // ENGINE_H
