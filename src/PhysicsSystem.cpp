#include "PhysicsSystem.h"

#include "Core/Math.h"
#include "Rendering/TransformComponent.h"
#include "GridComponent.h"
#include "PhysicsComponent.h"

PhysicsSystem::PhysicsSystem(EventManager *eventManager) : System(eventManager, TransformComponent::Type|PhysicsComponent::Type, 0)
{
    //ctor
}

PhysicsSystem::~PhysicsSystem()
{
    //dtor
}

void PhysicsSystem::begin(const float dt)
{
}

void PhysicsSystem::processEntity(Entity *entity, const float dt)
{
    auto trans = static_cast<TransformComponent*>(entity->getComponent(TransformComponent::Type));
    auto phys = static_cast<PhysicsComponent*>(entity->getComponent(PhysicsComponent::Type));

    if (phys->mGrid)
    {
        auto gt = static_cast<TransformComponent*>(phys->mGrid->getComponent(TransformComponent::Type));
        auto dist = gt->getPosition() - gt->getOrigin() - trans->getPosition();
        if (length(dist) >= 20000*16)
            phys->mGrid = NULL;
    }

    if (phys->mGrid == NULL)
    {
        for (auto g : mGrids)
        {
            auto gt = static_cast<TransformComponent*>(g->getComponent(TransformComponent::Type));
            auto dist = gt->getPosition() - gt->getOrigin() - trans->getPosition();
            if (length(dist) < 20000*16)
                phys->mGrid = g;
        }
    }

    if (phys->mGrid == NULL)
    {
        trans->move(phys->mVelocity*dt);
        return;
    }

    auto gt = static_cast<TransformComponent*>(phys->mGrid->getComponent(TransformComponent::Type));
    auto grid = static_cast<GridComponent*>(phys->mGrid->getComponent(GridComponent::Type));

    // Do gravity
    sf::Vector2f gravity;
    switch (phys->mGravityDir)
    {
    case DOWN:
        gravity.y = 4 * 9.8 * 16;
    }

    phys->mVelocity.y += gravity.y * dt;

    // Horizontal and vertical motion directions
    int hdir = NO_DIR;
    int vdir = NO_DIR;

    if (phys->mVelocity.x > 0)
        hdir = RIGHT;
    else if (phys->mVelocity.x < 0)
        hdir = LEFT;

    if (phys->mVelocity.y > 0)
        vdir = DOWN;
    else if (phys->mVelocity.y < 0)
        vdir = UP;

    phys->mDirCollisions[0] = false;
    phys->mDirCollisions[1] = false;
    phys->mDirCollisions[2] = false;
    phys->mDirCollisions[3] = false;

    // Handle collisions
    // Horizontal
    trans->move(sf::Vector2f(phys->mVelocity.x*dt, 0));
    if (hdir != NO_DIR)
    {
        float fix = 0;
        if (grid->checkCollision(gt, trans, sf::Vector2f(30, 96), hdir, fix))
        {
            phys->mVelocity.x = 0;
            trans->move(sf::Vector2f(fix, 0));
            phys->mDirCollisions[hdir] = true;
        }
    }

    // Vertical
    trans->move(sf::Vector2f(0, phys->mVelocity.y*dt));
    if (vdir != NO_DIR)
    {
        float fix = 0;
        if (grid->checkCollision(gt, trans, sf::Vector2f(30, 96), vdir, fix))
        {
            phys->mVelocity.y = 0;
            trans->move(sf::Vector2f(0, fix));
            phys->mDirCollisions[vdir] = true;
        }
    }
}

void PhysicsSystem::end(const float dt)
{
}
