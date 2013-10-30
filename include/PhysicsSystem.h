#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "Core/System.h"

class PhysicsSystem : public System
{
    public:
        PhysicsSystem(EventManager *eventManager);
        virtual ~PhysicsSystem();

        void addGrid(Entity* grid){mGrids.push_back(grid);}

    protected:
        void begin(const float dt);

        void processEntity(Entity *entity, const float dt);

        void end(const float dt);

    private:
        std::vector<Entity*> mGrids;
};

#endif // PHYSICSSYSTEM_H
