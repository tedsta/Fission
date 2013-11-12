#include <Fission/Core/Engine.h>

#include <Fission/Core/EventManager.h>
#include <Fission/Core/Entity.h>
#include <Fission/Core/System.h>
#include <Fission/Core/Scene.h>

#include <iostream>

Engine::Engine()
{
    mEventManager = new EventManager;
    mScene = new Scene(mEventManager);
}

Engine::~Engine()
{
    delete mScene;

    for (System *system : mSystems)
    {
        delete system;
    }

    delete mEventManager;
}

void Engine::update(const float dt)
{
    for (System *system : mSystems)
    {
        system->begin(dt);
        system->processEntities(dt);
        system->end(dt);
    }
}
