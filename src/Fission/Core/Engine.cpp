#include <Fission/Core/Engine.h>

#include <Fission/Core/EventManager.h>
#include <Fission/Core/System.h>
#include <Fission/Core/EntityManager.h>

#include <iostream>

namespace fsn
{
    Engine::Engine()
    {
        mEventManager = new EventManager;
        mEntityManager = new EntityManager(mEventManager);
    }

    Engine::~Engine()
    {
        delete mEntityManager;

        for (System* system : mSystems)
        {
            system->stop();
            delete system;
        }

        delete mEventManager;
    }

    void Engine::start()
    {
        for (System* system : mSystems)
        {
            system->start();
        }
    }
}

