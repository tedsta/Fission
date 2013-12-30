#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H

#include "Fission/Core/Component.h"

class TestComponent : public Component
{
    public:
        TestComponent() : mData(0)
        {
        }

        int mData;

        static ComponentType Type;
        ComponentType getType(){return Type;}
};

class Test2Component : public Component
{
    public:
        Test2Component() : mData(0)
        {
        }

        int mData;

        static ComponentType Type;
        ComponentType getType(){return Type;}
};

#endif // TESTCOMPONENT_H
