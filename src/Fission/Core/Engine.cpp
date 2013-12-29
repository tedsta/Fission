#include <Fission/Core/Engine.h>

#include <Fission/Core/EventManager.h>
#include <Fission/Core/System.h>
#include <Fission/Core/EntityManager.h>

#include <iostream>

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
        delete system;
    }

    delete mEventManager;
}

void Engine::update(const float dt)
{
    for (System *system : mSystems)
    {
        if (system->mLockStep <= 0)
        {
            system->begin(dt);
            system->processEntities(dt);
            system->end(dt);
        }
        else
        {
            system->mDtAccumulator += dt;

            while (system->mDtAccumulator >= system->mLockStep)
            {
                system->mDtAccumulator -= system->mLockStep;
                system->begin(system->mLockStep);
                system->processEntities(system->mLockStep);
                system->end(system->mLockStep);
            }
        }
    }
}
