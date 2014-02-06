#include <Fission/Script/ScriptSystem.h>

#include <stdio.h>
#include <stdarg.h>
#include <iostream>

#include <SFML/System/Vector2.hpp>
#include <Sqrat/sqext.h>

#include <Fission/Core/EntityRef.h>
#include <Fission/Core/EntityManager.h>
#include <Fission/Core/Engine.h>

namespace fsn
{
    void printfunc(HSQUIRRELVM v, const SQChar *s, ...)
    {
        va_list arglist;
        va_start(arglist, s);
        vprintf(s, arglist);
        va_end(arglist);
    }

    void bindSquirrel(HSQUIRRELVM vm, Engine *_engine)
    {
        sq_setprintfunc(vm, printfunc, NULL); // Sets the print function

        Sqrat::Class<sf::Vector2f, sqext::ConstAlloc<sf::Vector2f, float, float>> vector2(vm);
        vector2.Var("x", &sf::Vector2f::x);
        vector2.Var("y", &sf::Vector2f::y);
        Sqrat::RootTable(vm).Bind("Vector2f", vector2);

        Sqrat::Class<EntityRef> entity(vm);
        //entity.Func("addComponent", &EntityRef::addComponent);
        entity.Func("getComponent", (Component* (EntityRef::*)(ComponentType) const)&EntityRef::getComponent);
        entity.Func("getID", &EntityRef::getID);
        Sqrat::RootTable(vm).Bind("EntityRef", entity);

        Sqrat::Class<Component> component(vm);
        component.Func("getType", &Component::getType);
        Sqrat::RootTable(vm).Bind("Component", component);

        Sqrat::Class<EntityManager> entityManager(vm);
        entityManager.Func("createEntity", &EntityManager::createEntity);
        Sqrat::RootTable(vm).Bind("EntityManager", entityManager);

        Sqrat::Class<Engine> engine(vm);
        engine.Func("getEntityManager", &Engine::getEntityManager);
        Sqrat::RootTable(vm).Bind("Engine", engine);

        Sqrat::RootTable(vm).SetInstance("engine", _engine);
    }

    ScriptSystem::ScriptSystem(IEventManager *eventManager, Engine *engine) : System(eventManager),
        mVM(sq_open(1024)), mEngine(engine)
    {
        bindSquirrel(mVM, mEngine);
    }

    ScriptSystem::~ScriptSystem()
    {
        //sq_close(mVM);
    }

    HSQUIRRELVM ScriptSystem::createScript(const std::string& fileName)
    {
        HSQUIRRELVM vm = sq_open(1024);

        bindSquirrel(vm, mEngine);
        for (auto binder : mBinders)
            binder(vm);

        //load the script
        Sqrat::Script script(vm);

        std::string err;
        if (!script.CompileFile(fileName, err)) //there is an error
            std::cout << "Squirrel Compile Error: " << err << std::endl;
        else if (!script.Run(err))
            std::cout << "Squirrel Runtime Error: " << err << std::endl;

        return vm;
    }

    void ScriptSystem::processEntity(const EntityRef& entity, const float dt)
    {
    }
}

