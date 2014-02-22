#include <Fission/Core/Engine.h>

#include <Fission/Core/EventManager.h>
#include <Fission/Core/Systems/System.h>
#include <Fission/Core/EntityManager.h>
#include <Fission/Util/make_unique.h>

namespace fsn
{
    Engine::Engine(float lockStep) : mEventManager(), mEntityManager(),
        mLockStep(lockStep), mDtAccumulator(0.f)
    {
    }

    Engine::Engine(const fsn::Engine& other) : mEventManager(), mEntityManager(),
        mLockStep(other.mLockStep), mDtAccumulator(0.f)
    {
    }

    Engine::~Engine()
    {
    }

    void Engine::update(const float dt)
    {
        mEntityManager.lockEntityDestruction();

        if (mLockStep <= 0)
        {
            for (auto& system : mSystems)
            {
                system->update(dt);
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
                    system->update(mLockStep);
                }
            }
        }

        mEntityManager.unlockEntityDestruction();
    }

    void Engine::addSystem(System& system)
    {
        mSystems.push_back(&system);
    }
}

