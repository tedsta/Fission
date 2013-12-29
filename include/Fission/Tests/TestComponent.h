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
};

#endif // TESTCOMPONENT_H
