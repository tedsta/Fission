#include <Fission/Core/Engine.h>

#include <Fission/Core/EventManager.h>
#include <Fission/Core/System.h>
#include <Fission/Core/EntityManager.h>

#include <iostream>

namespace fsn
{
    Engine::Engine(float lockStep) : mEventManager(new EventManager), mEntityManager(new EntityManager(mEventManager.get())),
        mLockStep(lockStep), mDtAccumulator(0.f)
    {
    }

    Engine::Engine(const fsn::Engine& other) : mEventManager(new EventManager), mEntityManager(new EntityManager(mEventManager.get())),
        mLockStep(other.mLockStep), mDtAccumulator(0.f)
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::update(const float dt)
    {

        if (mLockStep <= 0)
        {
            for (auto& system : mSystems)
            {
                system->begin(dt);
                system->processEntities(dt);
                system->end(dt);
            }
        }
        else
        {
            mDtAccumulator += dt;

            while (mDtAccumulator >= mLockStep)
            {
                mDtAccumulator -= mLockStep;

                for (auto& system : mSystems)
                {
                    system->begin(mLockStep);
                    system->processEntities(mLockStep);
                    system->end(mLockStep);
                }
            }
        }
    }
}

