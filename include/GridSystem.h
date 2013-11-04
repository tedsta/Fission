#ifndef GRIDSYSTEM_H
#define GRIDSYSTEM_H

#include "Core/System.h"
#include "Network/Connection.h"
#include "GridComponent.h"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

// Grid operation function. The surrounding tiles and the physics ticks are passed
class GridOp
{
public:
    enum
    {
        STATIC,
        DYNAMIC
    };

    GridOp(Area (*stat)(Area)) : mType(STATIC), mStatic(stat), mDynamic(NULL) {}
    GridOp(void (*dyn)(GridComponent*, int)) : mType(DYNAMIC), mStatic(NULL), mDynamic(dyn) {}

    Area operator()(Area a) const
    {
        return mStatic(a);
    }

    void operator()(GridComponent* grid, int tick) const
    {
        mDynamic(grid, tick);
    }

    int getType() const {return mType;}

private:

    int mType;
    Area (*mStatic)(Area);
    void (*mDynamic)(GridComponent*, int);
};

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
};

#endif // GRIDSYSTEM_H
