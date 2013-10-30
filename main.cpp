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

#include "GridComponent.h"

int main()
{
    Engine *engine = new Engine;
    ResourceManager *rcMgr = new ResourceManager;

    TransformComponent::Type = ComponentFactories::add(TransformComponent::factory);
    SpriteComponent::Type = ComponentFactories::add(SpriteComponent::factory);
    ScriptComponent::Type = ComponentFactories::add(ScriptComponent::factory);
    IntentComponent::Type = ComponentFactories::add(IntentComponent::factory);
    GridComponent::Type = ComponentFactories::add(GridComponent::factory);

    Connection* conn = new Connection(engine->getEventManager());

    TestSystem *testSystem = new TestSystem(engine->getEventManager());
    RenderSystem *render = new RenderSystem(engine->getEventManager(), GridComponent::Type);
    InputSystem *input = new InputSystem(engine->getEventManager(), &render->getWindow());
    IntentSystem *intentSys = new IntentSystem(engine->getEventManager(), conn);
    ScriptSystem *scripting = new ScriptSystem(engine->getEventManager(), engine);

    engine->addSystem(testSystem);
    engine->addSystem(render);
    engine->addSystem(input);
    engine->addSystem(intentSys);
    engine->addSystem(scripting);

    GridComponent::addTileSheet(1, rcMgr->getTexture("Content/Textures/Tiles/dirt.png"));
    GridComponent::addTileSheet(2, rcMgr->getTexture("Content/Textures/Tiles/stone.png"));
    GridComponent::addTileSheet(3, rcMgr->getTexture("Content/Textures/Tiles/grass.png"));

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

    Tile** tiles = new Tile*[100];
    for (int y = 0; y < 100; y++)
    {
        tiles[y] = new Tile[100];
        for (int x = 0; x < 100; x++)
        {
            tiles[y][x].mMat = 1;
        }
    }

    for (int i = 0; i < 1; i++)
    {
        Entity *ground = new Entity(engine->getEventManager());
        scene->addEntity(ground);
        ground->addComponent(new TransformComponent(sf::Vector2f(0, 0)));
        ground->addComponent(new GridComponent(100, 100, tiles, 0));
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
