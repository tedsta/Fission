#include <iostream>
#include <bitset>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Core/Engine.h"
#include "Core/Scene.h"
#include "Core/ResourceManager.h"
#include "Core/ComponentFactories.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/SpriteComponent.h"
#include "Rendering/TransformComponent.h"

#include "Input/InputSystem.h"

#include "Network/Connection.h"
#include "Network/IntentSystem.h"
#include "Network/IntentComponent.h"

#include "Script/ScriptSystem.h"
#include "Script/ScriptComponent.h"

#include "TestSystem.h"

int main()
{
    Engine *engine = new Engine;
    ResourceManager *resourceMgr = new ResourceManager;

    TransformComponent::Type = ComponentFactories::add(TransformComponent::factory);
    SpriteComponent::Type = ComponentFactories::add(SpriteComponent::factory);
    ScriptComponent::Type = ComponentFactories::add(ScriptComponent::factory);
    IntentComponent::Type = ComponentFactories::add(IntentComponent::factory);

    Connection* conn = new Connection(engine->getEventManager());

    TestSystem *testSystem = new TestSystem(engine->getEventManager());
    RenderSystem *render = new RenderSystem(engine->getEventManager());
    InputSystem *input = new InputSystem(engine->getEventManager(), &render->getWindow());
    IntentSystem *intentSys = new IntentSystem(engine->getEventManager(), conn);
    ScriptSystem *scripting = new ScriptSystem(engine->getEventManager(), engine);

    engine->addSystem(testSystem);
    engine->addSystem(render);
    engine->addSystem(input);
    engine->addSystem(intentSys);
    engine->addSystem(scripting);

    Scene *scene = engine->getScene();

    sf::Texture texture;
    texture.loadFromFile("Content/Textures/robot.png");

    Entity *testEnt = new Entity(engine->getEventManager());
    scene->addEntity(testEnt);
    testEnt->addComponent(new TransformComponent);
    testEnt->addComponent(new SpriteComponent(&texture));
    testEnt->addComponent(new IntentComponent);
    testEnt->addComponent(new ScriptComponent(scripting->createScript("test.nut")));

    TransformComponent *trans = static_cast<TransformComponent*>(testEnt->getComponent(TransformComponent::Type));
    IntentComponent *intent = static_cast<IntentComponent*>(testEnt->getComponent(IntentComponent::Type));

    trans->setPosition(sf::Vector2f(3.f, 3.f));
    intent->mapKeyToIntent("up", sf::Keyboard::W, BtnState::DOWN);
    intent->mapKeyToIntent("down", sf::Keyboard::S, BtnState::DOWN);
    intent->mapKeyToIntent("left", sf::Keyboard::A, BtnState::DOWN);
    intent->mapKeyToIntent("right", sf::Keyboard::D, BtnState::DOWN);

    for (int i = 0; i < 1000; i++)
    {
        Entity *ground = new Entity(engine->getEventManager());
        scene->addEntity(ground);
        ground->addComponent(new TransformComponent(sf::Vector2f(i*0.1, 200.f)));
        ground->addComponent(new SpriteComponent(&texture));
        ground->addComponent(new ScriptComponent(scripting->createScript("test.nut")));
    }

    float accum = 0;
    int frames = 0;

    sf::Clock clock;
    while (render->getWindow().isOpen())
    {
        //while (clock.getElapsedTime().asMicroseconds() < 1000);

        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();
        accum += dt;

        if (accum >= 1.f)
        {
            std::cout << "FPS: " << frames << std::endl;
            accum = 0;
            frames = 0;
        }

        frames++;

        engine->update(dt);
    }

    delete engine;

    return 0;
}
