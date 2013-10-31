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

		for (auto coord : grid->mCTiles[t])
        {
			int x = coord.x;
			int y = coord.y;

			int left = grid->wrapX(x - 1);
            int right = grid->wrapX(x + 1);

            // Tiles
            Area a;
            a.mX = x;
            a.mY = y;
            if (y > 0)
            {
                a.mTiles[0][0] = grid->mTiles[y-1][left];
                a.mTiles[0][1] = grid->mTiles[y-1][x];
                a.mTiles[0][2] = grid->mTiles[y-1][right];
            }

            a.mTiles[1][0] = grid->mTiles[y][left];
            a.mTiles[1][1] = grid->mTiles[y][x];
            a.mTiles[1][2] = grid->mTiles[y][right];

            if (y < grid->mSizeY-1)
            {
                a.mTiles[2][0] = grid->mTiles[y+1][left];
                a.mTiles[2][1] = grid->mTiles[y+1][x];
                a.mTiles[2][2] = grid->mTiles[y+1][right];
            }

            // New tile
            a = mTicks[t].mOp(a);
            mAreas.push_back(a);
		}
		grid->mCTiles[t].clear();

		for (auto a : mAreas)
        {
			//left := grid->WrapX(a.x - 1)
			//right := grid->WrapX(a.x + 1)
			/*if (a.y > 0) {
				grid->SetTile(left, a.y-1, a.tiles[0][0], t)
				grid->SetTile(a.x, a.y-1, a.tiles[0][1], t)
				grid->SetTile(right, a.y-1, a.tiles[0][2], t)
			}*/

			//grid->SetTile(left, a.y, a.tiles[1][0], t)
			grid->setTile(a.mX, a.mY, a.mTiles[1][1], t);
			//grid->SetTile(right, a.y, a.tiles[1][2], t)

			/*if (a.y < grid->sizeY-1) {
				grid->SetTile(left, a.y+1, a.tiles[2][0], t)
				grid->SetTile(a.x, a.y+1, a.tiles[2][1], t)
				grid->SetTile(right, a.y+1, a.tiles[2][2], t)
			}*/
		}
		mAreas.clear();
	}
}

void GridSystem::end(const float dt)
{
}
