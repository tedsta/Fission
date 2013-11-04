#include "GridSystem.h"

#include <iostream>

GridSystem::GridSystem(EventManager *eventManager) : System(eventManager, GridComponent::Type)
{
    //ctor
}

GridSystem::~GridSystem()
{
    //dtor
}

void GridSystem::begin(const float dt)
{
}

void GridSystem::processEntity(Entity *entity, const float dt)
{
    auto grid = static_cast<GridComponent*>(entity->getComponent(GridComponent::Type));

	// ###############
	// Do the grid simulation

	if (grid->mID == -1)
    {
		grid->mID = mNextGridID;
		mNextGridID++;
	}

	// Iterate through cached interesting tiles that need to be operated on
	for (unsigned int t = 0; t < mTicks.size(); t++)
    {
		if (mTicks[t].mClock.getElapsedTime().asSeconds() < mTicks[t].mDelay)
			continue;

		mTicks[t].mClock.restart();

        //if (grid->mCTiles[t].size())
        //    std::cout << "Tick!\n";

        //std::cout << grid->mCTiles[t].size() << std::endl;
        if (mTicks[t].mOp.getType() == GridOp::STATIC)
        {
            std::vector<Area> areas;
            for (auto coord : grid->mCTiles[t])
            {
                Area a = grid->getArea(coord.x, coord.y);

                // New tile
                a = mTicks[t].mOp(a);
                if (a.mChanged)
                    areas.push_back(a);
            }

            grid->clearInteresting(t);

            //if (areas.size() > 0)
            //    std::cout << "Areas: " << areas.size() << std::endl;
            for (auto a : areas)
            {
                grid->setTile(a.mX, a.mY, a.mTiles[1][1], t);
            }
        }
        else
        {
            mTicks[t].mOp(grid, t);
            grid->clearInteresting(t);
        }
	}
}

void GridSystem::end(const float dt)
{
}
