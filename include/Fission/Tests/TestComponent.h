#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H

#include "Fission/Core/Component.h"

class TestComponent : public fsn::Component
{
    FISSION_COMPONENT

    public:
        TestComponent() : mData(0)
        {
        }

        int mData;
};

class Test2Component : public fsn::Component
{
    FISSION_COMPONENT

    public:
        Test2Component() : mData(0)
        {
        }

        int mData;
};

#endif // TESTCOMPONENT_H
