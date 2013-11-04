#include "PlayerSystem.h"

#include "Core/Math.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/TransformComponent.h"
#include "Network/IntentComponent.h"
#include "GridComponent.h"
#include "PlayerComponent.h"
#include "PhysicsComponent.h"

#include <iostream>

PlayerSystem::PlayerSystem(EventManager *eventManager, RenderSystem* rndSys) :
    System(eventManager, TransformComponent::Type|PlayerComponent::Type|PhysicsComponent::Type|IntentComponent::Type),
    mRndSys(rndSys)
{
    //ctor
}

PlayerSystem::~PlayerSystem()
{
    //dtor
}

void PlayerSystem::begin(const float dt)
{
}

void PlayerSystem::processEntity(Entity *entity, const float dt)
{
    auto trans = static_cast<TransformComponent*>(entity->getComponent(TransformComponent::Type));
    auto player = static_cast<PlayerComponent*>(entity->getComponent(PlayerComponent::Type));
    auto phys = static_cast<PhysicsComponent*>(entity->getComponent(PhysicsComponent::Type));
    auto intent = static_cast<IntentComponent*>(entity->getComponent(IntentComponent::Type));
    //auto skel = static_cast<SkeletonComponent*>(entity->getComponent(SkeletonComponent::Type));

	//skel.Apply(skel.FindAnimation("running"), player->manimTime, true)
	//skel.Update(dt)

	if (!player->mStupidMode)
    {
		if (intent->isIntentActive("left") && phys->getVelocity().x > -250)
		{
			phys->setVelocityX(-100);
			//skel.skel.FlipX = true
			player->mAnimTime += dt;
		}
        else if (intent->isIntentActive("right") && phys->getVelocity().x < 2500)
        {
			phys->setVelocityX(100);
			//skel.skel.FlipX = false
			player->mAnimTime += dt;
		}
		else
			phys->setVelocityX(0);
	}
	else
		phys->setVelocityX(100);

	if (!player->mStupidMode)
    {
		if (intent->isIntentActive("up") && phys->getVelocity().y > -250)
			phys->setVelocityY(-175);
	}
	else
    {
		if ((phys->getDirCollision(LEFT) || phys->getDirCollision(RIGHT)) && phys->getVelocity().y > -250)
			phys->setVelocityY(-175);
	}

	if (mRndSys)
    {
		if (player->mCam != trans->getPosition())
        {
			sf::Vector2f dist = trans->getPosition() - player->mCam;
			sf::Vector2f dir = normalize(dist);
			float camMove = length(dist) * 4 * dt;
			player->mCam += dir*camMove;
		}

		mRndSys->getView().setCenter(player->mCam);
	}

	if (intent->isIntentActive("stupidmode"))
		player->mStupidMode = !player->mStupidMode;

	if (phys->getGrid())
    {
		auto pt = static_cast<TransformComponent*>(phys->getGrid()->getComponent(TransformComponent::Type));
		auto grid = static_cast<GridComponent*>(phys->getGrid()->getComponent(GridComponent::Type));

		if (intent->isIntentActive("dig"))
        {
			sf::Vector2f mousePos = intent->getMousePos();
			mousePos += mRndSys->getView().getCenter();
			mousePos -= mRndSys->getView().getSize()/2.f;
			sf::Vector2f pos = grid->getTilePos(pt, mousePos);
			//if (length(mousePos - trans->getPosition()) < 16*10) //16 pixels per tile, 10 tiles
				grid->setTile(int(pos.x), int(pos.y), Tile(), -1);
		}
		if (intent->isIntentActive("place"))
		{
			sf::Vector2f mousePos = intent->getMousePos();
			mousePos += mRndSys->getView().getCenter();
			mousePos -= mRndSys->getView().getSize()/2.f;
			sf::Vector2f pos = grid->getTilePos(pt, mousePos);
			//if (length(mousePos - trans->getPosition()) < 16*10) //16 pixels per tile, 10 tiles
				grid->setTile(int(pos.x), int(pos.y), Tile(4, 0, 127), -1);
		}
	}
}

void PlayerSystem::end(const float dt)
{
}
