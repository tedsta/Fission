#ifndef GRIDSYSTEM_H
#define GRIDSYSTEM_H

#include "Core/System.h"
#include "Network/Connection.h"
#include "GridComponent.h"

#include <SFML/System/Clock.hpp>

struct Area
{
	Tile mTiles[3][3];
	int mX, mY;
	bool mMerge; // If true, add states, if false, replace tiles
};

// Grid operation function. The surrounding tiles and the physics ticks are passed
typedef Area (*GridOp)(Area);

struct Tick
{
    Tick(GridOp op, float delay) : mOp(op), mDelay(delay) {}

	GridOp mOp;
	float mDelay; // Delay between each tick in seconds
	sf::Clock mClock;
};

class GridSystem : public System
{
    public:
        GridSystem(EventManager *eventManager);
        virtual ~GridSystem();

        void addTick(GridOp op, float delay){mTicks.push_back(Tick(op, delay));}

    protected:
        void begin(const float dt);

        void processEntity(Entity *entity, const float dt);

        void end(const float dt);

    private:
        Connection *mConn; // Network connection
        int mHndID; // Network handle id
        std::vector<Tick> mTicks; // Ticks
        int mNextGridID; // Next grid id to give out
        std::vector<Area> mAreas;
};

#endif // GRIDSYSTEM_H
