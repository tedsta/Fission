#include <Fission/Script/ScriptSystem.h>

#include <stdio.h>
#include <stdarg.h>
#include <iostream>

#include <SFML/System/Vector2.hpp>
#include <Sqrat/sqext.h>

#include <Fission/Core/Entity.h>
#include <Fission/Core/Scene.h>
#include <Fission/Core/Engine.h>

#include <Fission/Script/ScriptComponent.h>

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

    Sqrat::Class<Entity> entity(vm);
    entity.Func("addComponent", &Entity::addComponent);
    entity.Func("addComponentSq", &Entity::addComponentSq);

    // Getters
    entity.Func("getID", &Entity::getID);
    entity.Func("getTypeBits", &Entity::getTypeBits);
    Sqrat::RootTable(vm).Bind("Entity", entity);

    Sqrat::Class<Component> component(vm);
    component.Func("getTypeBits", &Component::getTypeBits);
    Sqrat::RootTable(vm).Bind("Component", component);

    Sqrat::Class<Scene> scene(vm);
    scene.Func("createEntity", &Scene::createEntity);
    scene.Func("findEntity", &Scene::findEntity);
    scene.Func("clear", &Scene::clear);
    Sqrat::RootTable(vm).Bind("Scene", scene);

    Sqrat::Class<Engine> engine(vm);
    engine.Func("getScene", &Engine::getScene);
    Sqrat::RootTable(vm).Bind("Engine", engine);

    Sqrat::RootTable(vm).SetInstance("engine", _engine);
}

ScriptSystem::ScriptSystem(EventManager *eventManager, Engine *engine) : System(eventManager, ScriptComponent::Type), mVM(sq_open(1024)), mEngine(engine)
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

void ScriptSystem::processEntity(Entity *entity, const float dt)
{
    static_cast<ScriptComponent*>(entity->getComponent(ScriptComponent::Type))->executeUpdate(dt);
}
