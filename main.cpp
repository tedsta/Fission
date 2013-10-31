#include <iostream>
#include <bitset>
#include <cstdlib>

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


#include "GridComponent.h"
#include "PhysicsComponent.h"
#include "PlayerComponent.h"
#include "GridSystem.h"
#include "PhysicsSystem.h"
#include "PlayerSystem.h"

#include "GridOps.h"

#include "perlin/perlin.h"

Tile** newWorld(int seed);

int main()
{
    Engine *engine = new Engine;
    ResourceManager *rcMgr = new ResourceManager;

    TransformComponent::Type = ComponentFactories::add(TransformComponent::factory);
    SpriteComponent::Type = ComponentFactories::add(SpriteComponent::factory);
    ScriptComponent::Type = ComponentFactories::add(ScriptComponent::factory);
    IntentComponent::Type = ComponentFactories::add(IntentComponent::factory);
    GridComponent::Type = ComponentFactories::add(GridComponent::factory);
    PhysicsComponent::Type = ComponentFactories::add(PhysicsComponent::factory);
    PlayerComponent::Type = ComponentFactories::add(PlayerComponent::factory);

    Connection* conn = new Connection(engine->getEventManager());

    RenderSystem *render = new RenderSystem(engine->getEventManager(), GridComponent::Type);
    InputSystem *input = new InputSystem(engine->getEventManager(), &render->getWindow());
    IntentSystem *intentSys = new IntentSystem(engine->getEventManager(), conn);
    ScriptSystem *scripting = new ScriptSystem(engine->getEventManager(), engine);
    GridSystem *gridSys = new GridSystem(engine->getEventManager());
    PhysicsSystem *physSys = new PhysicsSystem(engine->getEventManager());
    PlayerSystem *playerSys = new PlayerSystem(engine->getEventManager(), render);

    engine->addSystem(render);
    engine->addSystem(input);
    engine->addSystem(intentSys);
    engine->addSystem(scripting);
    engine->addSystem(gridSys);
    engine->addSystem(physSys);
    engine->addSystem(playerSys);

    render->setBackgroundColor(sf::Color(130, 130, 255, 255));

    GridComponent::addTileSheet(1, rcMgr->getTexture("Content/Textures/Tiles/dirt.png"));
    GridComponent::addTileSheet(2, rcMgr->getTexture("Content/Textures/Tiles/stone.png"));
    GridComponent::addTileSheet(3, rcMgr->getTexture("Content/Textures/Tiles/grass.png"));

    gridSys->addTick(veggyGridOp, 1);

    Scene *scene = engine->getScene();

    sf::Texture texture;
    texture.loadFromFile("robot.png");

    Entity *player = new Entity(engine->getEventManager());
    scene->addEntity(player);
    player->addComponent(new TransformComponent);
    player->addComponent(new SpriteComponent(&texture));
    player->addComponent(new IntentComponent);
    player->addComponent(new PhysicsComponent);
    player->addComponent(new PlayerComponent);

    TransformComponent *trans = static_cast<TransformComponent*>(player->getComponent(TransformComponent::Type));
    IntentComponent *intent = static_cast<IntentComponent*>(player->getComponent(IntentComponent::Type));

    trans->setOrigin(sf::Vector2f(30, 48));
    trans->setPosition(sf::Vector2f(3.f, 3.f));
    intent->mapKeyToIntent("up", sf::Keyboard::W, BtnState::DOWN);
    intent->mapKeyToIntent("down", sf::Keyboard::S, BtnState::DOWN);
    intent->mapKeyToIntent("left", sf::Keyboard::A, BtnState::DOWN);
    intent->mapKeyToIntent("right", sf::Keyboard::D, BtnState::DOWN);

    intent->mapMouseBtnToIntent("dig", sf::Mouse::Button::Left, BtnState::DOWN);
    intent->mapMouseBtnToIntent("place", sf::Mouse::Button::Right, BtnState::DOWN);

    Tile** tiles = newWorld(0);

    for (int i = 0; i < 1; i++)
    {
        Entity *planet = new Entity(engine->getEventManager());
        scene->addEntity(planet);
        planet->addComponent(new TransformComponent(sf::Vector2f(0, 0)));
        planet->addComponent(new GridComponent(1000, 1000, tiles, 1));
        planet->addComponent(new ScriptComponent(scripting->createScript("test.nut")));
        physSys->addGrid(planet);
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

Tile** newWorld(int seed)
{
	int worldW = 1000;
	int worldH = 1000;
	Tile** tiles = new Tile*[worldH];
	for (int i = 0; i < worldH; i++)
		tiles[i] = new Tile[worldW];

	for (int y = 0; y < worldH; y++)
    {
		for (int x = 0; x < worldW; x++)
		{
			auto n = PerlinNoise1D(x, 1.01, .02, 2) + 1;
			auto p = PerlinNoise2D(y, x, 1.01, 0.2, 10) + 1;
			p += float(worldH-y) / float(worldH);
			float o[MAX_COMPS];
			for (int i = 0; i < MAX_COMPS; i++)
			{
				o[i] = PerlinNoise2D(x, y, 1.01, 0.2, i) + 1;
			}
			if (y > n*100)
			{
				tiles[y][x].mMat = p * 3 / 2;

				if (tiles[y][x].mMat > 3)
					tiles[y][x].mMat = 3;

				for (int i = 0; i < MAX_COMPS; i++)
                {
					tiles[y][x].mComp[i] = o[i] * 128;
				}
				tiles[y][x].mHeat = p * 256;
			}
		}
	}
	return tiles;
}
