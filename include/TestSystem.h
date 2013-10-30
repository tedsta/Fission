#ifndef TESTSYSTEM_H
#define TESTSYSTEM_H

#include "Core/System.h"

class TestSystem : public System
{
    public:
        TestSystem(EventManager *eventManager);
        virtual ~TestSystem();

    protected:
        void begin(const float dt);

        void processEntity(Entity *entity, const float dt);

        void end(const float dt);

    private:
};

#endif // TESTSYSTEM_H
