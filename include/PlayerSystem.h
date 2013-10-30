#ifndef PLAYERSYSTEM_H
#define PLAYERSYSTEM_H

#include "Core/System.h"

class RenderSystem;

class PlayerSystem : public System
{
    public:
        PlayerSystem(EventManager *eventManager, RenderSystem* rndSys);
        virtual ~PlayerSystem();

    protected:
        void begin(const float dt);

        void processEntity(Entity *entity, const float dt);

        void end(const float dt);

    private:
        RenderSystem* mRndSys;
};

#endif // PLAYERSYSTEM_H
